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
#include "../include/parse.hh"

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

  uint8_t errNumber;
  switch (argc) {
    case 2:
      directoryStructure.clear();
      errNumber = ParseDir(argv[1], directoryStructure, err);
      if (errNumber) {
        std::cout << errNumber;
        return errNumber;
      }
      [[fallthrough]];

    case 1:
      errNumber = Init(directoryStructure, err);
      reset(errNumber, err);
      errNumber = configOut(directoryStructure, err);
      reset(errNumber, err);
      break;

    default:
      std::cout << "Usage:\n" << argv[0] << "\n";
      std::cout << "        " << argv[0] << " <directory to copy from>\n";
      break;
  }

  std::cout << errNumber;
  return exitVal::success;
}

int Init(std::vector<directoryItem>& directoryStructure, std::error_code& err) {
  for (std::vector<directoryItem>::iterator it = directoryStructure.begin();
       it != directoryStructure.end(); ++it) {
    if (std::filesystem::exists(it->name)) {
      continue;
    }

    if (it->name.string().back() == '/' ||
        it->name.string().back() == '\\') {  // represents an empty directory
      if (!std::filesystem::create_directories(it->name, err)) {
        std::cerr << err.message();
        return exitVal::dirCreationFailed;
      }
    } else {  // represents a file
      if (!std::filesystem::exists(it->name.parent_path(), err) &&
          it->name.parent_path().empty()) {
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
