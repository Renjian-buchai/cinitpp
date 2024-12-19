#pragma once
#if !defined(CINITPP_ENUM_HH)
#define CINITPP_ENUM_HH

#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

enum flag_t : uint64_t {
  // Force creation, even if files are overwritten
  force = 0,

  // Create config from an existing file
  input = 1,

  // Select global configuration, for use or otherwise
  global = 2,

  // Config
  config = 3,

  flagSize = 4,
};

enum err_t : uint64_t {
  // Main errors
  errSuccess = 0,
  invalidFlags,
  invalidPath,
  nonEmptyDir,

  // Initialisation errors
  whereTfIsHome,

  // Autoconf errors
  nonexistentPath,
  creationError,

  // JSON errors
  missingContents,
  malformedJSON,
};

using dirItems = std::vector<std::pair<std::filesystem::path, std::string>>;
#define dirItemsDefault                                                        \
  {{"src/main.cpp", "int main() {}"},                                          \
   {"include/", ""},                                                           \
   {"README.md", ""},                                                          \
   {"CMakeLists.txt", ""}}

#endif