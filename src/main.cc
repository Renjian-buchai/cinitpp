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
  nonEmptyDir,
  configFileCreationFailed,
  configJsonInvalid,
  fileCreationFailed,
  dirCreationFailed,
  deletionFailed,
  testerr
};

int Init(std::vector<directoryItem>&, std::error_code&);

int main(int argc, char** argv) {
  std::error_code err{};

  (void)argc;
  (void)argv;
  (void)err;

  std::vector<directoryItem> directoryStructure{
      directoryItem("./build/"),
      directoryItem("./src/main.cc", "int main(int argc, char** argv) {}"),
      directoryItem("./include/external/"), directoryItem("./makefile")};

  // Ignores .git because it's considered a hidden directory.
  if (!std::filesystem::is_empty(".", err)) {
    std::cerr << "Unable to initialise in a non-empty repository.\n";
    return exitVal::nonEmptyDir;
  }

  if (argc == 1) {
    uint8_t errorNumber = Init(directoryStructure, err);

    // Deletes all files if an error occurs.
    if (errorNumber) {
      for (auto& dirEntry :
           std::filesystem::recursive_directory_iterator(".")) {
        if (!std::filesystem::remove_all(dirEntry.path(), err)) {
          std::cerr << err.message();
          return exitVal::deletionFailed;
        }
      }
    }

    return errorNumber;
  }

  std::cout << "Usage:\n" << std::string(argv[0]) << "\n";

  return exitVal::success;
}

int Init(std::vector<directoryItem>& directoryStructure, std::error_code& err) {
  for (std::vector<directoryItem>::iterator it = directoryStructure.begin();
       it != directoryStructure.end(); ++it) {
    if (std::filesystem::exists(it->name)) {
      continue;
    }

    if (it->name.string().back() == '/') {  // represents an empty directory
      if (!std::filesystem::create_directories(it->name, err)) {
        std::cerr << err.message();
        return exitVal::dirCreationFailed;
      }
    } else {  // represents a file
      if (!std::filesystem::create_directories(it->name.parent_path(), err)) {
        std::cerr << err.message();
        return exitVal::dirCreationFailed;
      }

      try {
        std::ofstream file{it->name};
        file << it->contents;
        file.close();
      }
      // Can't recall what error is thrown, so just catch a generic one.
      catch (std::exception& fe) {
        std::cerr << fe.what();
        return exitVal::fileCreationFailed;
      }
    }
  }
  return exitVal::success;
}
