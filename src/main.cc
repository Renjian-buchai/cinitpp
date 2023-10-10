#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/file.hh"

enum exitVal {
  success,
  configFileCreationFailed,
  configJsonInvalid,
  fileCreationFailed,
  dirCreationFailed
};

int Init(std::unordered_map<std::filesystem::path, std::vector<file>>& files);

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  std::unordered_map<std::filesystem::path, std::vector<file>> files{
      {"build", {}},
      {"src", {file("main.cc", "int main(int argc, char** argv) {}")}},
      {"include", {}},
      {".", {file("makefile")}}};

  if (argc == 1) {
    return Init(files);
  } else {
    std::cout << "Usage:\n" << std::string(argv[0]) << "\n";
  }

  return exitVal::success;
}

int Init(std::unordered_map<std::filesystem::path, std::vector<file>>& files) {
  for (auto it = files.begin(); it != files.end(); ++it) {
    if (!std::filesystem::exists(it->first)) {
      std::filesystem::create_directory(it->first);
    }

    if (!std::filesystem::exists(it->first)) {
      std::exit(exitVal::dirCreationFailed);
    }

    std::for_each(it->second.begin(), it->second.end(),
                  [&](file& paths) -> void {
                    std::ofstream _file{it->first / paths.name};

                    if (!std::filesystem::exists(it->first / paths.name)) {
                      std::exit(exitVal::fileCreationFailed);
                    }

                    _file << paths.contents;
                    _file.close();
                  });
  }

  return exitVal::success;
}
