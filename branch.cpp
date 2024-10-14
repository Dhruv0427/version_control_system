#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "my_vcs.h"

const std::string HEAD_FILE = ".vcs/HEAD";
const std::string BRANCHES_DIR = ".vcs/branches";
const std::string COMMITS_DIR = ".vcs/commits"; 

void list_branches() {
    std::cout << "Branches:" << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(BRANCHES_DIR)) {
        std::string branch_name = entry.path().filename().string();
        std::ifstream branch_file(entry.path());
        std::string commit_hash;

        if (branch_file.is_open()) {
            std::string line;
            while (std::getline(branch_file, line)) {
                commit_hash = line; 
            }
            std::cout << "- " << branch_name << " (Last Commit: " << commit_hash << ")" << std::endl;
        }
    }
}

void create_branch(const std::string& branch_name) {
    std::ifstream head_file(HEAD_FILE);
    std::string current_branch;

    if (head_file.is_open()) {
        std::getline(head_file, current_branch);
    } else {
        std::cerr << "Error: Unable to read HEAD file." << std::endl;
        return;
    }

    if (std::filesystem::exists(BRANCHES_DIR + "/" + branch_name)) {
        std::cerr << "Error: Branch '" << branch_name << "' already exists." << std::endl;
        return;
    }

    std::ofstream branch_file(BRANCHES_DIR + "/" + branch_name);
    if (!branch_file) {
        std::cerr << "Error: Unable to create branch." << std::endl;
        return;
    }

    std::ifstream current_branch_file(BRANCHES_DIR + "/" + current_branch);
    std::string current_commit;
    if (current_branch_file.is_open()) {
        std::string line;
        while (std::getline(current_branch_file, line)) {
            current_commit = line;
        }
        current_branch_file.close();
    }

    branch_file << current_commit << std::endl;
    branch_file.close();

    std::cout << "Branch '" << branch_name << "' created from commit: " << current_commit << std::endl;
}

void switch_branch(const std::string& branch_name) {
    std::ifstream branch_file(BRANCHES_DIR + "/" + branch_name);
    std::string commit_hash;

    if (branch_file.is_open()) {
        std::getline(branch_file, commit_hash); 
        
        std::ofstream head_file(HEAD_FILE); 
        head_file << branch_name; 
        head_file.close();
        
        std::cout << "Switched to branch '" << branch_name << "'." << std::endl;
    } else {
        std::cerr << "Error: Branch '" << branch_name << "' does not exist." << std::endl;
    }
}

void delete_branch(const std::string& branch_name) {
    if (branch_name == "master") {
        std::cerr << "Error: Cannot delete the main branch." << std::endl;
        return;
    }

    if (std::filesystem::remove(BRANCHES_DIR + "/" + branch_name)) {
        std::cout << "Branch '" << branch_name << "' deleted." << std::endl;
    } else {
        std::cerr << "Error: Branch '" << branch_name << "' does not exist or cannot be deleted." << std::endl;
    }
}

void handle_branch_command(const std::string& command, const std::string& branch_name = "") {
    if (command == "create") {
        create_branch(branch_name);
    } else if (command == "switch") {
        switch_branch(branch_name);
    } else if (command == "delete") {
        delete_branch(branch_name);
    } else if (command == "list") {
        list_branches();
    } else {
        std::cerr << "Error: Unknown command '" << command << "'." << std::endl;
    }
}
