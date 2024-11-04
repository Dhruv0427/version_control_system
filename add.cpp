#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";

void add(const std::string& filename) {
    // Ensure the staging directory exists
    if (!std::filesystem::exists(STAGING_DIR)) {
        std::filesystem::create_directories(STAGING_DIR);
    }

    // Define the file path relative to the current directory
    std::filesystem::path filepath = std::filesystem::current_path() / filename;

    // Check if the file exists and can be opened
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR: " << filepath << " does not exist or cannot be opened!" << std::endl;
        return;
    }
    
    // Define the destination path in the staging area
    std::filesystem::path destination = std::filesystem::path(STAGING_DIR) / filename;

    // Check if the file already exists in the staging area
    if (std::filesystem::exists(destination)) {
        std::cout << "FILE: " << filename << " is already present in the staging area." << std::endl;
        return;
    }

    // Copy the file to the staging directory, overwriting if it already exists
    std::filesystem::copy_file(filepath, destination, std::filesystem::copy_options::overwrite_existing);
    std::cout << "FILE: " << filename << " added to the staging area." << std::endl;

    return;
}
