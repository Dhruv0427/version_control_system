#include "my_vcs.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

const std::string STAGING_DIR = ".vcs/staging";
const std::string METADATA_FILE = ".vcs/METADATA";

void commit(std::string message) {
    if (!std::filesystem::exists(STAGING_DIR)) {
        std::cerr << "Staging area does not exist. No files to commit." << std::endl;
        return;
    }

    std::ofstream metadataFile(METADATA_FILE, std::ios::app);
    if (!metadataFile) {
        std::cerr << "Error opening METADATA file." << std::endl;
        return;
    }

    std::vector<std::string> committedFiles;

    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        std::string filename = entry.path().filename().string();
        diff(entry.path().string());
        committedFiles.push_back(filename);
    }

    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        std::filesystem::remove(entry.path());
    }

    size_t numberOfFiles = committedFiles.size();
    std::string fileNames;

    for (const auto& file : committedFiles) {
        fileNames += file + " ";
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream timeStream;
    timeStream << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    std::string commitTime = timeStream.str();

    metadataFile << "Number of files committed: " << numberOfFiles 
                 << " <" << fileNames << "> Message: " << message 
                 << " Timestamp: " << commitTime << std::endl;

    std::cout << "Commit completed and deltas created successfully." << std::endl;

    metadataFile.close();
}
