#if !defined(DIRECTORY_ITEM_HH)
#define DIRECTORY_ITEM_HH

#include <filesystem>
#include <string>

class directoryItem {
 protected:
 public:
  directoryItem();
  directoryItem(std::filesystem::path _name);
  directoryItem(std::filesystem::path _name, std::string _contents);

  std::filesystem::path name;
  std::string contents;
};

#endif
