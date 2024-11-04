#include "my_vcs.h"
#include <iostream>
#include <fstream>
#include <filesystem> // For std::filesystem

namespace fs = std::filesystem;

void storeDelta(const std::string &fileName, 
                const std::vector<std::string> &added, 
                const std::vector<std::pair<int, std::pair<std::string, std::string>>> &modified,  
                const std::vector<std::string> &deleted) {
    std::string deltaFile = getNextDeltaFileName(fileName); // Get the next delta file name
    std::ofstream deltaOut(deltaFile);

    // Writing added lines
    if (!added.empty()) {
        deltaOut << "Added Lines:\n";
        for (const auto &addedLine : added) {
            deltaOut << addedLine << "\n";
        }
    }

    // Writing modified lines
    if (!modified.empty()) {
        deltaOut << "\nModified Lines:\n";
        for (const auto &modifiedPair : modified) {
            deltaOut << "Line " << modifiedPair.first << ": from \"" 
                     << modifiedPair.second.first << "\" to \"" 
                     << modifiedPair.second.second << "\"\n";
        }
    }

    // Writing deleted lines
    if (!deleted.empty()) {
        deltaOut << "\nDeleted Lines:\n";
        for (const auto &deletedLine : deleted) {
            deltaOut << deletedLine << "\n";
        }
    }

    deltaOut.close();
    std::cout << "Delta changes saved to " << deltaFile << std::endl; // Output success message
}

void displayDelta(const std::string &deltaFile) {
    std::ifstream file(deltaFile);
    if (!file) {
        std::cerr << "Could not open delta file: " << deltaFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

std::string getNextDeltaFileName(const std::string &fileName) {
    // Create the delta directory for the file inside delta/<filename>
    std::string baseName = fs::path(fileName).stem().string(); // Get filename without extension
    std::string deltaDir = ".vcs/delta/" + baseName; // Use "delta/<filename>" for the directory
    if (!fs::exists(deltaDir)) {
        fs::create_directories(deltaDir);  // Create directory recursively if it doesn't exist
    }

    // Find the next available delta file number in the folder
    int versionNumber = 1;
    for (const auto &entry : fs::directory_iterator(deltaDir)) {
        std::string deltaFile = entry.path().filename().string();
        if (deltaFile.find("delta_") == 0) {  // Check if file starts with "delta_"
            size_t pos = deltaFile.find_last_of('_'); // Find the last underscore
            size_t ext = deltaFile.find_last_of('.');  // Find the last dot for extension
            if (pos != std::string::npos && ext != std::string::npos) {
                int currentVersion = std::stoi(deltaFile.substr(pos + 1, ext - pos - 1)); // Extract version number
                versionNumber = std::max(versionNumber, currentVersion + 1); // Increment for next version
            }
        }
    }

    // Return the next delta file name in the format delta/<filename>/delta_<version>.txt
    return deltaDir + "/delta_" + std::to_string(versionNumber) + ".txt"; // Construct file path
}
