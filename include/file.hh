#if !defined(FILE_HH)
#define FILE_HH

#include <filesystem>
#include <string>

class file {
 protected:
 public:
  file();
  file(std::filesystem::path _name);
  file(std::filesystem::path _name, std::string _contents);

  std::filesystem::path name;
  std::string contents;
};

#endif