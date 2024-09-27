#ifndef MY_VCS_H
#define MY_VCS_H

#include<string>

void init();
void add(const std::string& filename);
void commit(const std::string& message);
void log();

#endif
