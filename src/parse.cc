#include "../include/parse.hh"

#include <fstream>

#include "../include/enum.hh"

int ParseDir(std::filesystem::path dirToParse,
             std::vector<directoryItem>& target, std::error_code& err) {
  if (!std::filesystem::is_directory(dirToParse, err)) {
    return exitVal::copySrcNotADir;
  }

  uint8_t errNumber;
  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(dirToParse)) {
    errNumber = ParseItem(entry.path(), dirToParse, target, err);
    if (errNumber) {
      return errNumber;
    }
  }

  return exitVal::success;
}

int ParseItem(std::filesystem::path itemToParse, std::filesystem::path srcDir,
              std::vector<directoryItem>& target, std::error_code& err) {
  uint8_t errNumber;
  if (!std::filesystem::is_directory(itemToParse)) {
    target.push_back({});
    target.back().name = "." / std::filesystem::relative(itemToParse, srcDir);
    std::ifstream file{itemToParse};
    if (!file.is_open()) {
      return exitVal::parseFileOpeningFailed;
    }
    target.back().contents = std::string(std::istreambuf_iterator<char>(file),
                                         std::istreambuf_iterator<char>());
    file.close();

  } else {  // is a directory
    if (std::filesystem::is_empty(itemToParse)) {
      target.push_back({});
      target.back().name =
          "." / (std::filesystem::relative(itemToParse, srcDir)) / "";
      target.back().contents = "";

    } else {
      for (const auto& entry :
           std::filesystem::directory_iterator(itemToParse)) {
        errNumber = ParseItem(entry.path(), srcDir, target, err);
        if (errNumber) {
          return errNumber;
        }
      }
    }
  }
  return exitVal::success;
}
