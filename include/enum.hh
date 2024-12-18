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
  flagSize,
};

enum err_t : uint64_t {
  errSuccess = 0,
  invalidFlags,
  invalidPath,
  nonEmptyDir,

  whereTfIsHome,
  missingContents,
};

using dirItems = std::vector<std::pair<std::filesystem::path, std::string>>;

#endif