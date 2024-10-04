#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include "my_vcs.h"

const std::string COMMITS_DIR = ".vcs/commits";

std::pair<std::string, std::string> split_versioned_filename(const std::string& versioned_file) {
    size_t underscore_pos = versioned_file.find('_');
    std::string file_hash = versioned_file.substr(0, underscore_pos);
    std::string filename = versioned_file.substr(underscore_pos + 1);
    return {file_hash, filename};
}

void list_file_versions(const std::string& filename) {
    std::filesystem::path commit_dir(COMMITS_DIR);
    std::vector<std::string> versions;

    for (const auto& commit_entry : std::filesystem::directory_iterator(commit_dir)) {
        if (!std::filesystem::is_directory(commit_entry)) continue;  // Skip non-directory entries
        
        for (const auto& file_entry : std::filesystem::directory_iterator(commit_entry.path())) {
            auto [file_hash, file_name] = split_versioned_filename(file_entry.path().filename().string());

            if (file_name == filename) {
                versions.push_back(commit_entry.path().filename().string() + ": " + file_entry.path().filename().string());
            }
        }
    }

    if (versions.empty()) {
        std::cout << "No versions found for file: " << filename << std::endl;
        return;
    }

    std::cout << "Previous versions of " << filename << ":\n";
    for (size_t i = 0; i < versions.size(); ++i) {
        std::cout << i + 1 << ": " << versions[i] << std::endl;
    }
}

void restore_file_version(const std::string& filename, std::size_t version_number) { 
    std::filesystem::path commit_dir(COMMITS_DIR);
    std::vector<std::filesystem::path> version_paths;

    for (const auto& commit_entry : std::filesystem::directory_iterator(commit_dir)) {
        if (!std::filesystem::is_directory(commit_entry)) continue;

        for (const auto& file_entry : std::filesystem::directory_iterator(commit_entry.path())) {
            auto [file_hash, file_name] = split_versioned_filename(file_entry.path().filename().string());
            if (file_name == filename) {
                version_paths.push_back(file_entry.path());
            }
        }
    }

    if (version_number < 1 || version_number > version_paths.size()) {
        std::cout << "Invalid version number." << std::endl;
        return;
    }

    std::filesystem::copy_file(version_paths[version_number - 1], filename, std::filesystem::copy_options::overwrite_existing);
    std::cout << "Restored " << filename << " to version: " << version_paths[version_number - 1].filename().string() << std::endl;
}

void restore_file(const std::string& filename) {
    list_file_versions(filename);
    
    std::cout << "Enter the version number to restore: ";
    int version_number;
    std::cin >> version_number;
    
    restore_file_version(filename, version_number);
}
