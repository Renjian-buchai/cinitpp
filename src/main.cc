#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/external/nlohmann/json.hpp"
#include "../include/directoryItem.hh"

enum exitVal {
  success,
  configFileCreationFailed,
  configJsonInvalid,
  fileCreationFailed,
  dirCreationFailed
};

int Init(std::unordered_map<std::filesystem::path, std::vector<directoryItem>>& files);

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  std::unordered_map<std::filesystem::path, std::vector<directoryItem>> files{
      {"build", {}},
      {"src", {directoryItem("main.cc", "int main(int argc, char** argv) {}")}},
      {"include", {}},
      {".", {directoryItem("makefile")}}};

  if (argc == 1) {
    return Init(files);
  } else {
    std::cout << "Usage:\n" << std::string(argv[0]) << "\n";
  }

  return exitVal::success;
}

int Init(std::unordered_map<std::filesystem::path, std::vector<directoryItem>>& files) {
  for (auto it = files.begin(); it != files.end(); ++it) {
    if (!std::filesystem::exists(it->first)) {
      std::filesystem::create_directory(it->first);
    }

    if (!std::filesystem::exists(it->first)) {
      std::exit(exitVal::dirCreationFailed);
    }

    std::for_each(it->second.begin(), it->second.end(),
                  [&](directoryItem& paths) -> void {
                    std::ofstream _directoryItem{it->first / paths.name};

                    if (!std::filesystem::exists(it->first / paths.name)) {
                      std::exit(exitVal::fileCreationFailed);
                    }

                    _directoryItem << paths.contents;
                    _directoryItem.close();
                  });
  }

  return exitVal::success;
}
