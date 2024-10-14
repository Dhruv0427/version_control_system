#ifndef MY_VCS_H
#define MY_VCS_H

#include<string>
#include <unordered_map>
#include<vector>
#include<filesystem>


void init();
std::string calculate_sha1_hash(const std::string& content);
void add(const std::string& filename);
void log();

std::string generate_commit_hash(const std::vector<std::string>& file_hashes, const std::string& message, const std::string& prev_commit);
std::string create_blob(const std::filesystem::path& file_path);
std::string create_tree(const std::vector<std::pair<std::string, std::string>>& file_entries);
void commit(const std::string& message);

std::string get_commit_tree_hash(const std::string& commit_hash);
std::unordered_map<std::string, std::string> read_tree(const std::string& tree_hash);
void list_file_versions(const std::string& filename);
void restore_file_version(const std::string& commit_hash, const std::string& filename);
void restore_file(const std::string& filename);

void list_branches();
void create_branch(const std::string& branch_name);
void switch_branch(const std::string& branch_name);
void delete_branch(const std::string& branch_name);
void handle_branch_command(const std::string& command, const std::string& branch_name);


#endif
