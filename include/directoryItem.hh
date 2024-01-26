#if !defined(DIRECTORY_ITEM_HH)
#define DIRECTORY_ITEM_HH

#include <filesystem>
#include <string>

class directoryItem {
 protected:
 public:
  directoryItem();
  directoryItem(std::filesystem::path&& _name);
  directoryItem(std::filesystem::path&& _name, std::string&& _contents);

  /// @brief Full path of item
  /// @note If it ends in '/', it's a directory.
  std::filesystem::path name;

  /// @brief Contents of initial file
  std::string contents;
};

#endif
