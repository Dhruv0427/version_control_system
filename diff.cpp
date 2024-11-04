#include "my_vcs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Function to read the contents of a file and return it as a string
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

// Function to compare two file contents and store differences
void compareFiles(const std::string &currentFile, const std::string &prevFile) {
    std::string currentContent = readFile(currentFile);
    std::string previousContent = readFile(prevFile);
    
    std::vector<std::string> currentLines, previousLines;
    std::stringstream currentStream(currentContent);
    std::stringstream previousStream(previousContent);
    std::string line;

    // Read current file into lines
    while (std::getline(currentStream, line)) {
        currentLines.push_back(line);
    }

    // Read previous file into lines
    while (std::getline(previousStream, line)) {
        previousLines.push_back(line);
    }

    int addedLines = 0, modifiedLines = 0, deletedLines = 0;
    std::vector<std::string> added, deleted;
    std::vector<std::pair<int, std::pair<std::string, std::string>>> modified; // Line number, pair of original and modified lines

    // Compare current lines with previous lines
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

    // Display results
    std::cout << modifiedLines << " lines modified, "
              << addedLines << " lines added, "
              << deletedLines << " lines deleted." << std::endl;

    // Call storeDelta to save changes
    storeDelta(currentFile, added, modified, deleted);
}

// Main function to handle input and file operations
void diff(std::string fileName) {
    

    // Define the paths for the current and previous files
    std::string currentFile = fileName;
    std::string prevFile = ".vcs/delta/" + fs::path(fileName).stem().string() + "/prev_version.txt"; // Use the delta folder for the base

    // Create the delta folder for the file if it does not exist
    std::string folderName = ".vcs/delta/" + fs::path(fileName).stem().string(); // Get the filename without extension
    if (!fs::exists(folderName)) {
        fs::create_directories(folderName);
        std::cout << "Directory created: " << folderName << std::endl;
    }

    // If the previous version does not exist, create it
    if (!fs::exists(prevFile)) {
        fs::copy(currentFile, prevFile);
        std::cout << "Initial version saved." << std::endl;
    } else {
        compareFiles(currentFile, prevFile); // Compare the files if the previous version exists
        fs::copy(currentFile, prevFile, fs::copy_options::overwrite_existing); // Update previous version with the latest commit
    }

}
