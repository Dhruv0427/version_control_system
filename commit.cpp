#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";
const std::string METADATA_FILE = ".vcs/commits/metadata.txt";
const std::string COMMITS_DIR = ".vcs/commits";
const std::string BLOBS_DIR = ".vcs/blobs";
const std::string TREES_DIR = ".vcs/trees";
const std::string HEAD_FILE = ".vcs/HEAD";
const std::string BRANCHES_DIR = ".vcs/branches";

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

std::string create_blob(const std::filesystem::path& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();

    std::string file_hash = calculate_sha1_hash(file_content);
    std::string blob_path = BLOBS_DIR + "/" + file_hash;

    std::ofstream blob_file(blob_path, std::ios::binary);
    blob_file << file_content;
    blob_file.close();

    return file_hash; 
}

std::string create_tree(const std::vector<std::pair<std::string, std::string>>& file_entries) {
    std::ostringstream tree_content;
    for (const auto& [blob_hash, filename] : file_entries) {
        tree_content << blob_hash << " " << filename << std::endl;
    }

    std::string tree_hash = calculate_sha1_hash(tree_content.str());
    std::string tree_path = TREES_DIR + "/" + tree_hash;

    std::ofstream tree_file(tree_path);
    tree_file << tree_content.str();
    tree_file.close();

    return tree_hash;
}

void commit(const std::string& message) {

    if (!std::filesystem::exists(COMMITS_DIR)) {
        std::filesystem::create_directory(COMMITS_DIR);
    }
    if (!std::filesystem::exists(BLOBS_DIR)) {
        std::filesystem::create_directory(BLOBS_DIR);
    }
    if (!std::filesystem::exists(TREES_DIR)) {
        std::filesystem::create_directory(TREES_DIR);
    }
    if (!std::filesystem::exists(BRANCHES_DIR)) {
        std::filesystem::create_directory(BRANCHES_DIR);
    }   

    std::ofstream metadata(METADATA_FILE, std::ios::app);
    if (!metadata) {
        std::cerr << "Error opening metadata file!" << std::endl;
        return;
    }

    std::string current_branch = "master"; 
    if (std::filesystem::exists(HEAD_FILE)) {
        std::ifstream head_file(HEAD_FILE);
        std::getline(head_file, current_branch);
    }

    std::string prev_commit;
    std::ifstream branch_file(BRANCHES_DIR + "/" + current_branch);
    if (branch_file.is_open()) {
        std::string line;
        while (std::getline(branch_file, line)) {
            prev_commit = line;  
        }
    }

    std::vector<std::pair<std::string, std::string>> file_entries; 

    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        std::string filename = entry.path().filename().string();
        std::string blob_hash = create_blob(entry.path());
        file_entries.push_back({blob_hash, filename});
        std::cout << "Blob created: " << blob_hash << std::endl;
    }

    std::string tree_hash = create_tree(file_entries);
    std::cout << "Tree created with hash: " << tree_hash << std::endl;

    std::vector<std::string> tree_hashes = { tree_hash };
    std::string commit_hash = generate_commit_hash(tree_hashes, message, prev_commit);

    if (!message.empty()) {
        metadata << commit_hash << " - Branch: " << current_branch << " - Tree: " << tree_hash << " - Message: " << message << std::endl;
        std::cout << "Commit Message: " << message << std::endl;
    } else {
        metadata << commit_hash << " - Branch: " << current_branch << " - Tree: " << tree_hash << " - Message: [No message]" << std::endl;
    }

    std::ofstream branch_commit_file(BRANCHES_DIR + "/" + current_branch, std::ios::app);
    branch_commit_file << commit_hash << std::endl; 
    branch_commit_file.close();

    std::ofstream head_file(HEAD_FILE);
    head_file << current_branch; 
    head_file.close();

    std::cout << "Commit created with hash: " << commit_hash << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)) {
        std::filesystem::remove(entry.path());
    }
}
