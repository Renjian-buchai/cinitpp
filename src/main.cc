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
  copySrcNotADir,
  parseFileOpeningFailed,
  testerr
};

int Init(std::vector<directoryItem>& directoryStructure);
int ParseDir(std::filesystem::path dirToParse, std::vector<directoryItem> *target);
int ParseItem(std::filesystem::path itemToParse, std::filesystem::path srcDir,
                std::vector<directoryItem> *target);

int main(int argc, char** argv) {
  std::vector<directoryItem> directoryStructure{
      directoryItem("build/"),
      directoryItem("src/main.cc", "int main(int argc, char** argv) {}"),
      directoryItem("include/external/"), directoryItem("./makefile")};

  if (!std::filesystem::is_empty(".")) {
    std::cerr << "Unable to initialise in a non-empty repository.\n"
                 "This will not be because you have initialised a git\n"
                 "repository prior to running this program.\n";
    return exitVal::nonEmptyDir;
  }

  uint8_t err;
  switch (argc) {
    case 2:
      directoryStructure.clear();
      err = ParseDir(argv[1], &directoryStructure);
      if (err) {
        return err;
      }
      [[fallthrough]];

    case 1:  
      err = Init(directoryStructure);
      if (err) {
        for (auto& dirEntry :
             std::filesystem::recursive_directory_iterator(".")) {
          std::filesystem::remove_all(dirEntry.path());
        }
      }
      break;

    default:
      std::cout << "Usage:\n" << argv[0] << "\n";
      std::cout << "        " << argv[0] << " <directory to copy from>\n";
      break;
  }

  return exitVal::success;
}

int Init(std::vector<directoryItem>& directoryStructure) {
  for (auto it = directoryStructure.begin(); it != directoryStructure.end();
       ++it) {
    it->name = std::filesystem::current_path() / it->name;

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

int ParseDir(std::filesystem::path dirToParse, std::vector<directoryItem> *target) {
  if (!std::filesystem::is_directory(dirToParse)) {
    return exitVal::copySrcNotADir;
  }

  uint8_t err;
  for (const auto& entry : std::filesystem::directory_iterator(dirToParse)) {
    err = ParseItem(entry.path(), dirToParse, target);
    if (err) {
      return err;
    }
  }

  return exitVal::success;
}

int ParseItem(std::filesystem::path itemToParse, std::filesystem::path srcDir, std::vector<directoryItem> *target) {
  uint8_t err;
  if (!std::filesystem::is_directory(itemToParse)) {
    target->push_back({});
    target->back().name = std::filesystem::relative(itemToParse, srcDir);
    std::ifstream file{itemToParse};
    if (!file.is_open()) {
      return exitVal::parseFileOpeningFailed;
    }
    target->back().contents = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();

  } else { //is a directory
    if (std::filesystem::is_empty(itemToParse)) {
      target->push_back({});
      target->back().name = (std::filesystem::relative(itemToParse, srcDir)).string() + "/";
      target->back().contents = "";

    } else {
      for (const auto& entry : std::filesystem::directory_iterator(itemToParse)) {
        err = ParseItem(entry.path(), srcDir, target);
	if (err) {
	  return err;
	}
      }

    }
  }

  return exitVal::success;
} 
