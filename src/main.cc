#include "../include/main.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

enum exitVal {
  success,
  configFileCreationFailed,
  configJsonInvalid,
  fileCreationFailed,
  dirCreationFailed
};

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  if (argc == 1) {
    return Init();
  } else {
    std::cout << "Usage:\n" << std::string(argv[0]) << "\n";
  }

  return exitVal::success;
}

auto rdConfig() {
  nlohmann::basic_json ret{};
  std::ifstream config("~/.cinitppConfig.json");

  if (config) {
    config >> ret;

    if (nlohmann::json::accept(ret)) {
      std::cerr << "Invalid JSON. Stop fucking around with it manually.\n";
      std::exit(exitVal::configJsonInvalid);
    }
  } else {
    std::ofstream{"~/.cinitppConfig.json"};

    if (!std::filesystem::exists("~/.cinitppConfig.json")) {
      std::cerr << "Unable to generate config file. Try using sudo mode.\n";
      std::exit(exitVal::configFileCreationFailed);
    }
  }

  return ret;
}

int Init() {
  namespace fileSystem = std::filesystem;
  int ret = 0;

  fileSystem::create_directory("./build");
  if (!fileSystem::exists("./build")) {
    std::cout << "3";
    return exitVal::dirCreationFailed;
  }

  fileSystem::create_directory("./src");
  if (!fileSystem::exists("./src")) {
    std::cout << "1.1";
    return exitVal::fileCreationFailed;
  }

  fileSystem::create_directory("./include");
  if (!fileSystem::exists("./include")) {
    std::cout << "2.2";
    return exitVal::fileCreationFailed;
  }

  std::ofstream file{"makefile"};
  if (!file.is_open()) {
    std::cout << "4";
    return exitVal::fileCreationFailed;
  }
  file.close();

  file.open("./src/main.cc");
  if (!file.is_open()) {
    std::cout << "1";
    return exitVal::fileCreationFailed;
  }
  file << "int main(int argc, const char** argv) {}\n";
  file.close();

  return ret;
}
