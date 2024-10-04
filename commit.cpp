#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <vector>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";
const std::string METADATA_FILE = ".vcs/commits/metadata.txt";
const std::string COMMITS_DIR = ".vcs/commits";
const std::string HEAD_FILE = ".vcs/HEAD";

std::string calculate_sha1_hash(const std::string& input) {
    std::hash<std::string> hasher;
    size_t hash = hasher(input);
    
    std::stringstream hash_string;
    hash_string << std::hex << std::setw(16) << std::setfill('0') << hash;
    return hash_string.str();
}

std::string generate_commit_hash(const std::vector<std::string>& file_hashes, const std::string& message, const std::string& prev_commit) {
    std::ostringstream combined_data;
    for (const auto& hash : file_hashes) {
        combined_data << hash;
    }
    combined_data << message << prev_commit;
    
    return calculate_sha1_hash(combined_data.str());
}

void commit(const std::string& message) {
    std::filesystem::path vcsDir(COMMITS_DIR);
    if (!std::filesystem::exists(vcsDir)) {
        std::filesystem::create_directory(vcsDir);
    }

    std::ofstream metadata(METADATA_FILE, std::ios::app);
    if (!metadata) {
        std::cerr << "Error opening metadata file!" << std::endl;
        return;
    }

    std::vector<std::string> file_hashes;
    std::string commit_hash_dir; 

    std::string prev_commit;
    if (std::filesystem::exists(HEAD_FILE)) {
        std::ifstream head_file(HEAD_FILE);
        std::getline(head_file, prev_commit);
    }

    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        std::string filename = entry.path().filename().string();
        
        std::ifstream file(entry.path(), std::ios::binary);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string file_content = buffer.str();
        std::string file_hash = calculate_sha1_hash(file_content);  
        file_hashes.push_back(file_hash);

        if (commit_hash_dir.empty()) {
            std::string commit_hash = generate_commit_hash(file_hashes, message, prev_commit);
            commit_hash_dir = COMMITS_DIR + "/" + commit_hash;
            std::filesystem::create_directory(commit_hash_dir);
        }

        std::string versioned_filename = file_hash + "_" + filename;
        std::filesystem::copy_file(entry.path(), commit_hash_dir + "/" + versioned_filename, std::filesystem::copy_options::overwrite_existing);

        std::cout << "Committed: " << versioned_filename << std::endl;
    }

    std::string commit_hash = generate_commit_hash(file_hashes, message, prev_commit);

    if (!message.empty()) {
        metadata << commit_hash << " - Message: " << message << std::endl;
        std::cout << "Commit Message: " << message << std::endl;
    } else {
        metadata << commit_hash << " - Message: [No message]" << std::endl;
    }

    std::ofstream head_file(HEAD_FILE);
    head_file << commit_hash;

    std::filesystem::remove_all(STAGING_DIR);
    std::filesystem::create_directory(STAGING_DIR);

    metadata.close();
    head_file.close();

    std::cout << "Commit Hash: " << commit_hash << std::endl;
}

