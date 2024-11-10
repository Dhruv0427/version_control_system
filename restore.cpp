#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include "my_vcs.h"

// Function to apply reverse changes from a delta file
void applyReverseDelta(const std::string &deltaFile, std::vector<std::string> &lines) {
    std::ifstream delta(deltaFile);
    if (!delta.is_open()) {
        std::cerr << "Failed to open " << deltaFile << std::endl;
        return;
    }

    std::string line;
    std::regex modifiedLineRegex(R"(Modified Lines:)");
    std::regex modifiedRegex("Line (\\d+): from \"(.*)\" to \"(.*)\"");
    std::regex deletedLineRegex(R"(Deleted Lines:)");
    std::regex deletedRegex(R"(Line (\d+): (.*))");
    std::regex addedLineRegex(R"(Added Lines:)");
    std::regex addedRegex(R"(Line (\d+): (.*))");

    // Process the delta file
    while (getline(delta, line)) {
        std::smatch match;

        if (std::regex_match(line, modifiedLineRegex)) {
            // Process Modified lines
            while (getline(delta, line) && std::regex_match(line, match, modifiedRegex)) {
                auto lineNum = static_cast<std::vector<std::string>::size_type>(std::stoi(match[1]) - 1); // 0-based index
                std::string fromText = match[2];
                std::string toText = match[3];

                // Reverse: change `to` text back to `from` text
                if (lineNum < lines.size() && lines[lineNum] == toText) {
                    std::cout << "Restoring modified line: " << lineNum + 1 << " from \"" << toText << "\" to \"" << fromText << "\"\n";
                    lines[lineNum] = fromText; // Restore modified line
                } else {
                    std::cout << "Skipping modified line at " << lineNum + 1 << ": Current content does not match expected 'to' text.\n";
                }
            }
        } else if (std::regex_match(line, deletedLineRegex)) {
            // Process Deleted lines
            std::vector<std::pair<int, std::string>> toInsert;
            while (getline(delta, line) && std::regex_match(line, match, deletedRegex)) {
                int lineNum = std::stoi(match[1]) - 1; // 0-based index
                std::string deletedText = match[2];
                toInsert.push_back({lineNum, deletedText});
            }
            // Insert deleted lines in reverse order to maintain correct indexing
            for (auto it = toInsert.rbegin(); it != toInsert.rend(); ++it) {
                auto lineNum = static_cast<std::vector<std::string>::size_type>(it->first);
                std::cout << "Restoring deleted line: " << lineNum + 1 << " with text \"" << it->second << "\"\n";
                if (lineNum <= lines.size()) {
                    lines.insert(lines.begin() + lineNum, it->second); // Insert at the correct position
                }
            }
        } else if (std::regex_match(line, addedLineRegex)) {
            // Process Added lines
            std::vector<int> toErase;
            while (getline(delta, line) && std::regex_match(line, match, addedRegex)) {
                int lineNum = std::stoi(match[1]) - 1; // 0-based index
                toErase.push_back(lineNum);
            }
            // Erase added lines in reverse order to maintain correct indexing
            for (auto it = toErase.rbegin(); it != toErase.rend(); ++it) {
                auto lineNum = static_cast<std::vector<std::string>::size_type>(*it);
                if (lineNum < lines.size()) {
                    std::cout << "Removing added line at: " << lineNum + 1 << "\n";
                    lines.erase(lines.begin() + lineNum); // Remove added lines
                }
            }
        }
    }
    delta.close();
}

// Function to restore a file to a specified version
void restoreToVersion(const std::string &fileName, int targetVersion) {
    std::vector<std::string> lines;
    std::ifstream currentFile(".vcs/delta/" + fileName + "/prev_version.txt");
    if (!currentFile.is_open()) {
        std::cerr << "Failed to open the current version file." << std::endl;
        return;
    }

    std::string line;
    while (getline(currentFile, line)) {
        lines.push_back(line);
    }
    currentFile.close();

    // Read the current version from versions.txt
    int currentVersion;
    std::ifstream versionFile(".vcs/delta/" + fileName + "/versions.txt");
    if (versionFile.is_open()) {
        std::string versionText;
        getline(versionFile, versionText);
        if (std::sscanf(versionText.c_str(), "NUMBER OF VERSIONS :- %d", &currentVersion) != 1) {
            std::cerr << "Error parsing number of versions." << std::endl;
            return;
        }
        versionFile.close();
    } else {
        std::cerr << "Failed to open versions.txt" << std::endl;
        return;
    }

    for (int i = currentVersion; i > targetVersion; --i) {
        std::string deltaFile = ".vcs/delta/" + fileName + "/delta_" + std::to_string(i - 1) + ".txt";

        if (std::filesystem::exists(deltaFile)) {
            applyReverseDelta(deltaFile, lines);
        } else {
            std::cerr << "Delta file " << deltaFile << " does not exist. Skipping." << std::endl;
        }
    }

    std::ofstream restoredFile(fileName);
    if (!restoredFile.is_open()) {
        std::cerr << "Failed to open the file for restoring." << std::endl;
        return;
    }
    for (const std::string &restoredLine : lines) {
        restoredFile << restoredLine << "\n";
    }
    restoredFile.close();

    std::cout << "File restored to version " << targetVersion << ".\n";
}

void restore() {
    std::string fileName;
    int targetVersion;

    std::cout << "Enter the name of the file to restore: ";
    std::cin >> fileName;

    int numVersions;
    std::ifstream versionFile(".vcs/delta/" + fileName + "/versions.txt");
    if (versionFile.is_open()) {
        std::string versionText;
        getline(versionFile, versionText);
        if (std::sscanf(versionText.c_str(), "NUMBER OF VERSIONS :- %d", &numVersions) != 1) {
            std::cerr << "Error parsing number of versions." << std::endl;
            return;
        }
        versionFile.close();
    } else {
        std::cerr << "Failed to open versions.txt" << std::endl;
        return;
    }

    std::cout << "The file has " << numVersions << " versions available.\n";
    std::cout << "Which version would you like to restore to? ";
    std::cin >> targetVersion;
    if (targetVersion <= 0 || targetVersion > numVersions) {
        std::cerr << "Invalid version number.\n";
        return;
    }

    restoreToVersion(fileName, targetVersion);
}
