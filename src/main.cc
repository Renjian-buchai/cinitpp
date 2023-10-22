#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/directoryItem.hh"
#include "../include/enum.hh"
#include "../include/errHandler.hh"
#include "../include/external/nlohmann/json.hpp"
#include "../include/json.hh"

int Init(std::vector<directoryItem>&, std::error_code&);
int ParseDir(std::filesystem::path dirToParse,
             std::vector<directoryItem>* target);
int ParseItem(std::filesystem::path itemToParse, std::filesystem::path srcDir,
              std::vector<directoryItem>* target);

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

  configOut(directoryStructure, err);

  uint8_t errNumber;
  switch (argc) {
    case 2:
      directoryStructure.clear();
      errNumber = ParseDir(argv[1], &directoryStructure);
      if (errNumber) {
        return errNumber;
      }
      [[fallthrough]];

    case 1:
      errNumber = Init(directoryStructure, err);
      reset(errNumber, err);
      errNumber = configOut(directoryStructure, err);
      break;

    default:
      std::cout << "Usage:\n" << argv[0] << "\n";
      std::cout << "        " << argv[0] << " <directory to copy from>\n";
      break;
  }

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
      if (!std::filesystem::exists(it->name.parent_path(), err)) {
        if (!std::filesystem::create_directories(it->name.parent_path(), err)) {
          std::cerr << err.message();
          return exitVal::dirCreationFailed;
        }
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

int ParseDir(std::filesystem::path dirToParse,
             std::vector<directoryItem>* target) {
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

int ParseItem(std::filesystem::path itemToParse, std::filesystem::path srcDir,
              std::vector<directoryItem>* target) {
  uint8_t err;
  if (!std::filesystem::is_directory(itemToParse)) {
    target->push_back({});
    target->back().name = std::filesystem::relative(itemToParse, srcDir);
    std::ifstream file{itemToParse};
    if (!file.is_open()) {
      return exitVal::parseFileOpeningFailed;
    }
    target->back().contents = std::string(std::istreambuf_iterator<char>(file),
                                          std::istreambuf_iterator<char>());
    file.close();

  } else {  // is a directory
    if (std::filesystem::is_empty(itemToParse)) {
      target->push_back({});
      target->back().name =
          (std::filesystem::relative(itemToParse, srcDir)).string() + "/";
      target->back().contents = "";

    } else {
      for (const auto& entry :
           std::filesystem::directory_iterator(itemToParse)) {
        err = ParseItem(entry.path(), srcDir, target);
        if (err) {
          return err;
        }
      }
    }
  }
  return exitVal::success;
}
