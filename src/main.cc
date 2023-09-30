#include "../include/main.hh"

#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  Init();
}

int Init() {
  enum {
    srcFailed = 1,
    headerFailed = 1 << 1,
    buildDirFailed = 1 << 2,
    MakefileFailed = 1 << 3
  };

  namespace fileSystem = std::filesystem;
  int ret = 0;

  fileSystem::create_directory("./build");
  if (!fileSystem::exists("./build")) {
    std::cout << "3";
    return buildDirFailed;
  }

  fileSystem::create_directory("./src");
  if (!fileSystem::exists("./src")) {
    std::cout << "1.1";
    return srcFailed;
  }

  fileSystem::create_directory("./include");
  if (!fileSystem::exists("./include")) {
    std::cout << "2.2";
    return headerFailed;
  }

  std::ofstream file{"makefile"};
  if (!file.is_open()) {
    std::cout << "4";
    return MakefileFailed;
  }
  file.close();

  file.open("./src/main.cc");
  if (!file.is_open()) {
    std::cout << "1";
    return srcFailed;
  }
  file << "int main(int argc, const char** argv) {}\n";
  file.close();

  return ret;
}
