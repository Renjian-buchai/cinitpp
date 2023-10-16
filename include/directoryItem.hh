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


  std::filesystem::path name; //If the name ends with a '/',
			      //this item is an empty directory.
			      //Otherwise, this item is a file.

  std::string contents;       //If this item is an empty directory,
			      //'contents' is an empty std::string.
};

#endif
