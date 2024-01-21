#if !defined(CINITPP_DIR_HH)
#define CINITPP_DIR_HH

#include <variant>
#include <vector>

#include "../include/file.hh"

class directory {
 public:
  using filesystem_t = std::variant<directory, file>;

  directory() = delete;
  directory(directory* prev, std::filesystem::path path);

  const std::filesystem::path& path();
  const directory* prev();

  std::vector<filesystem_t> _inDir;

 protected:
  std::filesystem::path _path;
  directory* _prev;
};

#endif