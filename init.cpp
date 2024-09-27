#include<iostream>
#include<filesystem>
#include "my_vcs.h"

void init(){
if(!std::filesystem::exists(".vcs")) {
    std::filesystem::create_directory(".vcs");
    std::filesystem::create_directory(".vcs/commit");
    std::filesystem::create_directory(".vcs/staging");
    
    std::cout << "Directory structure '.vcs' created in the directory: " 
              << std::filesystem::current_path() << std::endl;
    std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
}
  else{
    std::cout<<"'.vcs' file already exist"<<std::endl;
  }

  return;
}
