#include "delta.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <ctime>

void generateUniqueDelta(const std::vector<std::string>& oldLines, const std::vector<std::string>& newLines) {
    if(!std::filesystem::exists("delta"))
        std::filesystem::create_directories("delta");

    std::time_t currentTime = std::time(nullptr);
    std::string deltaFileName = "delta/delta_" + std::to_string(currentTime) + ".txt";

    std::ofstream deltaFile(deltaFileName);
    if (!deltaFile.is_open()) {
        std::cerr << "Error opening delta file." << std::endl;
        return;
    }

    deltaFile << "Delta Changes:\n";

    for (size_t i = 0; i < std::max(oldLines.size(), newLines.size()); ++i) {
        if (i < oldLines.size() && i < newLines.size()) {
            if (oldLines[i] != newLines[i]) {
                deltaFile << "Type: modify, Position: " << (i + 1)
                          << ", Old: \"" << oldLines[i]
                          << "\", New: \"" << newLines[i] << "\"\n";
            }
        } else if (i < oldLines.size()) {
            deltaFile << "Type: delete, Position: " << (i + 1)
                      << ", Old: \"" << oldLines[i] << "\"\n";
        } else {
            deltaFile << "Type: add, Position: " << (i + 1)
                      << ", New: \"" << newLines[i] << "\"\n";
        }
    }

    deltaFile.close();
}
