#ifndef MY_VCS_H
#define MY_VCS_H

#include<string>

void init();
void add(const std::string& filename);
std::string generate_commit_hash(const std::vector<std::string>& file_hashes, const std::string& message, const std::string& prev_commit);
std::string calculate_sha1_hash(const std::string& input);
void commit(const std::string& message);
void log();

void restore_file(const std::string& filename);
void restore_file_version(const std::string& filename, std::size_t version_number);
void list_file_versions(const std::string& filename);
std::pair<std::string, std::string> split_versioned_filename(const std::string& versioned_file);

#endif
