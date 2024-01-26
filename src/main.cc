#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <variant>

#include "../include/dir.hh"
#include "../include/file.hh"

enum class flag_t : uint8_t {
  // Input run, i.e. don't run cinitpp
  input,
  // Force initialise, even if dir is not empty
  force
};

uint8_t flipTrue(uint8_t& flags, flag_t flag) {
  return flags | (1 << static_cast<uint8_t>(flag));
}

uint8_t flipFalse(uint8_t& flags, flag_t flag) {
  return flags & ~(1 << static_cast<uint8_t>(flag));
}

uint8_t checkTrue(uint8_t flags, flag_t flag) {
  return flags & (1 << static_cast<uint8_t>(flag));
}

// Don't touch
void recurse(const directory* currDir) {
  static std::ofstream fstr;
  // using filesystem_t = std::variant<directory, file>;
  for (size_t i = 0; i < currDir->_inDir.size(); ++i) {
    if (std::holds_alternative<directory>(currDir->_inDir[i])) {
      std::filesystem::create_directories(
          std::get_if<directory>(&(currDir->_inDir[i]))->_path);

      recurse(std::get_if<directory>(&(currDir->_inDir[i])));
    } else {
      fstr.open(currDir->_path /
                std::get_if<file>(&currDir->_inDir[i])->filePath);
      fstr << std::get_if<file>(&currDir->_inDir[i])->_contents;
      fstr.close();
    }
  }

  return;
}

int main(int argc, const char** argv) {
  (void)argc, (void)argv;

  std::string buffer;

  uint8_t flags;

  switch (argc) {
    case 1:
      break;

    default:
      for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-I") {
          buffer = argv[++i];
          flipTrue(flags, flag_t::input);
        }

        else if (std::string(argv[i]) == "-F") {
          flipTrue(flags, flag_t::force);
        }
      }
  }

  if (checkTrue(flags, flag_t::input)) {
  }

  if (!checkTrue(flags, flag_t::force)) {
    if (!std::filesystem::is_empty(std::filesystem::current_path())) {
      std::cerr << "Unable to initialise in a non-empty directory. To force "
                   "the initialisation, use the flag `-f`.\n"
                   "The .git/ folder is also not allowed.";
      return 1;
    }
  }

  // using filesystem_t = std::variant<directory, file>;

  /**
   * @note All directories must contain the relative path from the root
   * directory, all files should just contain its name.
   */
  directory root{"./"};
  root._inDir = {file{"makefile"}, directory{"./aoeu"}};
  std::get_if<directory>(&root._inDir[1])->_inDir = {
      file{"Wtf", "aoeuaoeuaoeu"}};
  directory* currDir = &root;

  recurse(currDir);

  return 0;
}
