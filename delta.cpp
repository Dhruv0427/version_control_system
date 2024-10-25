#include "delta.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void storeDelta(const std::string &fileName, 
                const std::vector<std::string> &added, 
                const std::vector<std::pair<int, std::pair<std::string, std::string>>> &modified,  
                const std::vector<std::string> &deleted) {
    std::string deltaFile = getNextDeltaFileName(fileName);
    std::ofstream deltaOut(deltaFile);

    if (!added.empty()) {
        deltaOut << "Added Lines:\n";
        for (const auto &addedLine : added) {
            deltaOut << addedLine << "\n";
        }
    }

    if (!modified.empty()) {
        deltaOut << "\nModified Lines:\n";
        for (const auto &modifiedPair : modified) {
            deltaOut << "Line " << modifiedPair.first << ": from \"" 
                     << modifiedPair.second.first << "\" to \"" 
                     << modifiedPair.second.second << "\"\n";
        }
    }

    if (!deleted.empty()) {
        deltaOut << "\nDeleted Lines:\n";
        for (const auto &deletedLine : deleted) {
            deltaOut << deletedLine << "\n";
        }
    }

    deltaOut.close();
    std::cout << "Delta changes saved to " << deltaFile << std::endl;
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
    std::string baseName = fs::path(fileName).stem().string();
    std::string deltaDir = "delta/" + baseName;
    if (!fs::exists(deltaDir)) {
        fs::create_directories(deltaDir);
    }

    int versionNumber = 1;
    for (const auto &entry : fs::directory_iterator(deltaDir)) {
        std::string deltaFile = entry.path().filename().string();
        if (deltaFile.find("delta_") == 0) {
            size_t pos = deltaFile.find_last_of('_');
            size_t ext = deltaFile.find_last_of('.');
            if (pos != std::string::npos && ext != std::string::npos) {
                int currentVersion = std::stoi(deltaFile.substr(pos + 1, ext - pos - 1));
                versionNumber = std::max(versionNumber, currentVersion + 1);
            }
        }
    }

    return deltaDir + "/delta_" + std::to_string(versionNumber) + ".txt";
}
