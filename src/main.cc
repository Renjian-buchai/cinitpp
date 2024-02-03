#include <cstdint>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <variant>

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

int main(int argc, const char** argv) {
  (void)argc, (void)argv;

  std::string buffer;

  uint8_t flags = 0;

  switch (argc) {
    case 1:
      break;

    default:
      for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-I") {
          buffer = argv[++i];
          flags = flipTrue(flags, flag_t::input);
        }

        else if (std::string(argv[i]) == "-F") {
          flags = flipTrue(flags, flag_t::force);
        }
      }
  }

  // This one is in a scope because getting input is the hard part.
  if (!checkTrue(flags, flag_t::input)) {
    if (!checkTrue(flags, flag_t::force)) {
      if (!std::filesystem::is_empty(std::filesystem::current_path())) {
        std::cerr << "Unable to initialise in a non-empty directory.\n"
                     "To force the initialisation, use the flag `-f`.\n"
                     "This will delete the data contained in files that are "
                     "created.\n\n"
                     "The project must not have been initialised to an git "
                     "repository already.\n";
        return 1;
      }
    }

    std::vector<file> files;
    files = {file{"./aoeu.txt", ""}, file{"./src/aoeu.txt", ""}};

    std::ofstream _file;
    for (auto file : files) {
      std::filesystem::create_directories(file.filePath.parent_path());
      _file.open(file.filePath);
      _file << file._contents;
      _file.close();
    }
    return 0;
  }
  // Validate input path
  if (!buffer.empty()) {
    std::filesystem::path inputPath(buffer);
    if (!std::filesystem::is_directory(inputPath)) {
      std::cout << "Input path must be a directory.\n"
                   "Please verify its existence.\n";
      return 1;
    }

    std::string _path;
    std::ifstream _fileIn;
    std::string __buffer;
    std::string _buffer;

    __buffer = "[";

    for (const std::filesystem::directory_entry& entry :
         std::filesystem::recursive_directory_iterator(inputPath)) {
      if (!std::filesystem::is_directory(entry)) {
        _path = std::filesystem::relative(entry.path()).string();
        std::replace(_path.begin(), _path.end(), '\\', '/');

        _fileIn.open(_path);
        _buffer = "";
        while (std::getline(_fileIn, buffer)) {
          _buffer += buffer + "\\n";
        }
        _fileIn.close();

        __buffer +=
            "{\"Path\":\"" + _path + "\", \"contents\":\"" + _buffer + "\"},";
      }
    }
    __buffer.pop_back();
    __buffer += "]";

    nlohmann::json _new = nlohmann::json::parse(__buffer);

    std::ofstream config;
    config.open("./.cinitpp.json");
    config << std::setw(4) << _new;
    config.close();
  }

  return 0;
}
