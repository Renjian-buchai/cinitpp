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
  dirCreationFailed,
  nonEmptyDir,
  testerr
};

int Init(std::vector<directoryItem>& directoryStructure);

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  std::vector<directoryItem> directoryStructure{
      directoryItem("./build/"),
      directoryItem("./src/main.cc", "int main(int argc, char** argv) {}"),
      directoryItem("./include/external/"), directoryItem("./makefile")};

  if (!std::filesystem::is_empty(".")) {
    std::cerr << "Unable to initialise in a non-empty repository.\n"
                 "This will not be because you have initialised a git\n"
                 "repository prior to running this program.\n";
    return exitVal::nonEmptyDir;
  }

  if (argc == 1) {
    uint8_t err = Init(directoryStructure);

    if (err) {
      for (auto& dirEntry :
           std::filesystem::recursive_directory_iterator(".")) {
        std::filesystem::remove_all(dirEntry.path());
      }
    }

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

      try {
        std::filesystem::create_directories(it->name);
      } catch (std::filesystem::filesystem_error& fe) {
        std::cerr << fe.what();
        return exitVal::dirCreationFailed;
      }
    } else {  // represents a file

      try {
        std::filesystem::create_directories(it->name.parent_path());
      } catch (std::filesystem::filesystem_error& fe) {
        return exitVal::dirCreationFailed;
      }

      try {
        std::ofstream file{it->name};
        file << it->contents;
        file.close();
      } catch (std::filesystem::filesystem_error& fe) {
        std::cerr << fe.what();
        return exitVal::fileCreationFailed;
      }
    }
  }

  return exitVal::success;
}
