#include "my_vcs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void compareFiles(const std::string &currentFile, const std::string &prevFile) {
    std::string currentContent = readFile(currentFile);
    std::string previousContent = readFile(prevFile);
    
    std::vector<std::string> currentLines, previousLines;
    std::stringstream currentStream(currentContent);
    std::stringstream previousStream(previousContent);
    std::string line;

    while (std::getline(currentStream, line)) {
        currentLines.push_back(line);
    }

    while (std::getline(previousStream, line)) {
        previousLines.push_back(line);
    }

    int addedLines = 0, modifiedLines = 0, deletedLines = 0;
    std::vector<std::string> added, deleted;
    std::vector<std::pair<int, std::pair<std::string, std::string>>> modified;

    size_t maxLength = std::max(currentLines.size(), previousLines.size());
    for (size_t i = 0; i < maxLength; ++i) {
        if (i < previousLines.size() && i < currentLines.size()) {
            if (currentLines[i] != previousLines[i]) {
                modifiedLines++;
                modified.push_back({static_cast<int>(i + 1), {previousLines[i], currentLines[i]}});
            }
        } else if (i < currentLines.size()) {
            addedLines++;
            added.push_back("Line " + std::to_string(i + 1) + ": " + currentLines[i]);
        } else if (i < previousLines.size()) {
            deletedLines++;
            deleted.push_back("Line " + std::to_string(i + 1) + ": " + previousLines[i]);
        }
    }

    if (modifiedLines == 0 && addedLines == 0 && deletedLines == 0) {
        std::cout << "No changes made in this version." << std::endl;
        return;
    }

    std::cout << modifiedLines << " lines modified, "
              << addedLines << " lines added, "
              << deletedLines << " lines deleted." << std::endl;

    storeDelta(currentFile, added, modified, deleted);
}

void diff(std::string fileName) {
    std::string currentFile = fileName;
    std::string prevFile = ".vcs/delta/" + fs::path(fileName).filename().string() + "/prev_version.txt";
    std::string folderName = ".vcs/delta/" + fs::path(fileName).filename().string();

    if (!fs::exists(folderName)) {
        fs::create_directories(folderName);
        std::cout << "Directory created: " << folderName << std::endl;
    }

    if (!fs::exists(prevFile)) {
        fs::copy(currentFile, prevFile);
        std::cout << "Initial version saved." << std::endl;
    } else {
        compareFiles(currentFile, prevFile);
        fs::copy(currentFile, prevFile, fs::copy_options::overwrite_existing);
    }
}
