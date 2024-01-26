#include "../include/parse.hh"

#include <fstream>

#include "../include/enum.hh"

int ParseDir(std::filesystem::path dirToParse,
             std::vector<directoryItem>& target, std::error_code& err) {
  if (!std::filesystem::is_directory(dirToParse, err)) {
    return exitVal::copySrcNotADir;
  }

  std::ifstream file;

  for (const std::filesystem::directory_entry& entry :
       std::filesystem::recursive_directory_iterator(dirToParse)) {
    if (!std::filesystem::is_directory(entry)) {  // Not a dir (i.e. a file)
      target.push_back({"." / std::filesystem::relative(entry, dirToParse)});

      file.open(entry.path());
      if (!file.is_open()) {
        return exitVal::parseFileOpeningFailed;
      }

      target.back().contents = std::string(std::istreambuf_iterator(file),
                                           std::istreambuf_iterator<char>());

      file.close();
    } else {
      target.push_back({
        static_cast<std::filesystem::path>(
            ("." / std::filesystem::relative(entry, dirToParse)).string() +
#if defined(_WIN32) || defined(_WIN64)
            "\\"
#else
            "/"
#endif
            ),
      });
    }
  }

  return exitVal::success;
}