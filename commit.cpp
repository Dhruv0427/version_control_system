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
    // Check if staging directory exists
    if (!std::filesystem::exists(STAGING_DIR)) {
        std::cerr << "Staging area does not exist. No files to commit." << std::endl;
        return;
    }

    // Open METADATA file for appending
    std::ofstream metadataFile(METADATA_FILE, std::ios::app);
    if (!metadataFile) {
        std::cerr << "Error opening METADATA file." << std::endl;
        return;
    }

    std::vector<std::string> committedFiles; // Vector to store names of committed files

    // Process each file in the staging directory and log to METADATA
    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        // Use std::filesystem::path to get just the filename
        std::string filename = entry.path().filename().string(); // Get just the filename
        
        // Call diff function to create deltas
        diff(entry.path().string()); // Pass the full path to the diff function

        // Store the filename for logging
        committedFiles.push_back(filename);
    }

    // Clear the staging area after commit
    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        std::filesystem::remove(entry.path());
    }

    // Prepare the metadata entry
    size_t numberOfFiles = committedFiles.size();
    std::string fileNames;

    for (const auto& file : committedFiles) {
        fileNames += file + " "; // Append file names to the string
    }

    // Get the current time for the timestamp
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream timeStream;
    timeStream << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    std::string commitTime = timeStream.str();

    // Write to the METADATA file with timestamp
    metadataFile << "Number of files committed: " << numberOfFiles 
                 << " <" << fileNames << "> Message: " << message 
                 << " Timestamp: " << commitTime << std::endl;

    std::cout << "Commit completed and deltas created successfully." << std::endl;

    // Close METADATA file
    metadataFile.close();
}
