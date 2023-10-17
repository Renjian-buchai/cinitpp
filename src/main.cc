#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/directoryItem.hh"
#include "../include/external/nlohmann/json.hpp"

enum exitVal {
  success,
  configFileCreationFailed,
  configJsonInvalid,
  fileCreationFailed,
  dirCreationFailed
};

int Init(std::vector<directoryItem>& directoryStructure);

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  std::vector<directoryItem> directoryStructure{
      directoryItem("./build/"),
      directoryItem("./src/main.cc", "int main(int argc, char** argv) {}"),
      directoryItem("./include/"), directoryItem("./makefile")};

  if (argc == 1) {
    return Init(directoryStructure);
  } else {
    std::cout << "Usage:\n" << std::string(argv[0]) << "\n";
  }

  return exitVal::success;
}

int Init(std::vector<directoryItem>& directoryStructure) {
  for (auto it = directoryStructure.begin(); it != directoryStructure.end();
       ++it) {
    if (std::filesystem::exists(it->name)) {
      continue;
    }

    if (it->name.string().back() == '/') {  // represents an empty directory
      it->name = it->name.parent_path();    // remove trailing '/'

      std::filesystem::create_directories(it->name);
      if (!std::filesystem::exists(it->name)) {
        return exitVal::dirCreationFailed;
      }
    } else {  // represents a file
      std::filesystem::create_directories(it->name.parent_path());
      if (!std::filesystem::exists(it->name.parent_path())) {
        return exitVal::dirCreationFailed;
      }

      std::ofstream file{it->name};
      if (!file.is_open()) {
        return exitVal::fileCreationFailed;
      }

      file << it->contents;
      file.close();
    }
  }

  return exitVal::success;
}
