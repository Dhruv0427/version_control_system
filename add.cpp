#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";

void add(const std::string& filename) {
    if (!std::filesystem::exists(STAGING_DIR)) {
        std::filesystem::create_directories(STAGING_DIR);
    }

    std::filesystem::path filepath = std::filesystem::current_path() / filename;

    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR: " << filepath << " does not exist or cannot be opened!" << std::endl;
        return;
    }
    
    std::filesystem::path destination = std::filesystem::path(STAGING_DIR) / filename;

    if (std::filesystem::exists(destination)) {
        std::cout << "FILE: " << filename << " is already present in the staging area." << std::endl;
        return;
    }

    std::filesystem::copy_file(filepath, destination, std::filesystem::copy_options::overwrite_existing);
    std::cout << "FILE: " << filename << " added to the staging area." << std::endl;
}
