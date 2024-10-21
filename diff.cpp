#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include "delta.h"


void readFileLines(const std::string& filename, std::vector<std::string>& lines) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
}

void compareFiles(const std::vector<std::string>& oldLines, const std::vector<std::string>& newLines) {
    int oldSize = oldLines.size();
    int newSize = newLines.size();
    int modified = 0, added = 0, deleted = 0;

    for (int i = 0; i < std::max(oldSize, newSize); ++i) {
        if (i < oldSize && i < newSize) {
            if (oldLines[i] != newLines[i]) {
                modified++;
                std::cout << "Modified Line " << i + 1 << ": \"" << oldLines[i] << "\" to \"" << newLines[i] << "\"\n";
            }
        } else if (i < oldSize) {
            deleted++;
            std::cout << "Deleted Line " << i + 1 << ": \"" << oldLines[i] << "\"\n";
        } else {
            added++;
            std::cout << "Added Line " << i + 1 << ": \"" << newLines[i] << "\"\n";
        }
    }

    std::cout << modified << " line(s) modified.\n";
    std::cout << added << " line(s) added.\n";
    std::cout << deleted << " line(s) deleted.\n";

    if (modified > 0 || added > 0 || deleted > 0) {
        generateUniqueDelta(oldLines, newLines); 
    } else {
        std::cout << "No changes were made.\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string versionDir = "versions/";
    std::string deltaDir = "delta/";

    if(!std::filesystem::exists(versionDir))
        std::filesystem::create_directories(versionDir);

    if(!std::filesystem::exists(deltaDir))
        std::filesystem::create_directories(deltaDir);

    if (std::filesystem::exists(versionDir + filename)) {
        std::cout << "Base version of file \"" << filename << "\" exists.\n";

        std::vector<std::string> oldLines;
        std::vector<std::string> newLines;

        readFileLines(versionDir + filename, oldLines); 
        readFileLines(filename, newLines);              

        compareFiles(oldLines, newLines); 
    } else {
        std::filesystem::copy_file(filename, versionDir + filename);
        std::cout << "Base version of file \"" << filename << "\" added.\n";
    }

    return 0;
}
