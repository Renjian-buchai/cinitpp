#include <cstdint>
#include <cstdlib>
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

  std::filesystem::path homeDir;

#if defined(__unix__)
  const char* dirPointer = std::getenv("HOME");
  homeDir = dirPointer ? dirPointer : "";
  if (homeDir.empty()) {
    std::cout << "Unable to find home directory to generate configuration. "
                 "\nPlease make sure your HOME environment variable is "
                 "properly configured. ";
    return 2;
  }
#elif defined(_WIN32)
  const char* dirPointer = std::getenv("USERPROFILE");
  homeDir = dirPointer ? dirPointer : "";
  if (homeDir.empty()) {
    const char* homeDrive = std::getenv("HOMEDRIVE");
    const char* homePath = std::getenv("HOMEPATH");
    homeDir = std::string(homeDrive ? homeDrive : "") + std::getenv(homePath ? homePath : "");
  }
#endif

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

    if (!std::filesystem::exists(homeDir / ".cinitpp.json")) {
      files = {
          file{"./makefile", ""},
          file{"./src/main.cc",
               "int main(int argc, const char** argv, const char** envp) {\n"
               "\t(void)argc, (void)argv, (void)envp;\n"
               "\n"
               "\treturn 0;"
               "\n}"},
          file{"./README.md", ""}};
    } else {
      std::ifstream input(homeDir / ".cinitpp.json");
      nlohmann::json config;
      try {
        config = nlohmann::json::parse(input);
      } catch (nlohmann::json::parse_error& e) {
        std::cout << "Invalid JSON";
        return 4;
      }

      if (!config.is_array()) {
        std::cout << "JSON is in the wrong format.";
        return 5;
      }

      files.push_back({std::filesystem::path("./") / config[1]["Path"],
                       config[1]["Content"]});

      try {
        for (auto it : config) {
          files.push_back(file{it["Path"].get<std::string>(),
                               it["Content"].get<std::string>()});
        }
      } catch (nlohmann::json::exception& e) {
        std::cout << "JSON is in the wrong format.";
        return 5;
      }
    }

    std::ofstream _file;
    for (auto file : files) {
      std::filesystem::create_directories(file.filePath.parent_path());
      _file.open(file.filePath);
      _file << file._contents;
      _file.close();
    }

    return 0;
  }

  {
    // Input

    // Validate input path
    if (!buffer.empty()) {
      std::filesystem::path inputPath(buffer);
      if (!std::filesystem::is_directory(inputPath)) {
        std::cout << "Input path must be a directory.\n"
                     "Please verify its existence.\n";
        return 3;
      }

      std::string _path;
      std::ifstream _fileIn;
      std::string _buffer;
      nlohmann::json config = nlohmann::json::array();

      for (const std::filesystem::directory_entry& entry :
           std::filesystem::recursive_directory_iterator(inputPath)) {
        if (!std::filesystem::is_directory(entry)) {
          _path = std::filesystem::relative(entry.path()).string();
          std::replace(_path.begin(), _path.end(), '\\', '/');

          _fileIn.open(_path);
          _buffer = "";
          while (std::getline(_fileIn, buffer)) {
            _buffer += buffer + "\n";
          }
          _fileIn.close();

          config.push_back(nlohmann::json::object(
              {{"Path", static_cast<std::filesystem::path>(".") / _path},
               {"Content", _buffer}}));
        }
      }

      std::ofstream output(homeDir / ".cinitpp.json");
      output << std::setw(4) << config;
      output.close();
    }

    return 0;
  }
}
