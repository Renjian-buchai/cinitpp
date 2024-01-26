#if !defined(CINITPP_FILE_HH)
#define CINITPP_FILE_HH

#include <filesystem>
#include <string>

class file {
 public:
  file() = delete;
  file(std::filesystem::path path);
  file(std::filesystem::path path, std::string contents);

  const std::filesystem::path& path();
  const std::string& contents();

  std::filesystem::path filePath;
  std::string _contents;
};

#endif