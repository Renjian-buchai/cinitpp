#include "input.hh"
#include "config/cfgWriter.hh"
#include "enum.hh"
#include "util.hh"
#include <filesystem>
#include <fstream>

err_t autoConf(const std::vector<bool> &flags, const inputData_t &inputData,
               std::string &err) {
  namespace stdfs = std::filesystem;

  if (!stdfs::exists(inputData.inputPath)) {
    std::cerr << "Path `" << inputData.inputPath.string()
              << "` does not exist\n"
                 "Exiting...\n\n";
    return err_t::nonexistentPath;
  }

  if (!stdfs::is_directory(inputData.inputPath)) {
    std::cerr << "Path `" << inputData.inputPath.string()
              << "` is not a directory.\n"
                 "Exiting...\n\n";
    return err_t::nonexistentPath;
  }

  dirItems items;

  for (const stdfs::directory_entry &it :
       stdfs::recursive_directory_iterator(inputData.inputPath)) {
    if (stdfs::is_directory(it)) {
      if (stdfs::is_empty(it)) {
        items.emplace_back(
            it.path().lexically_relative(inputData.inputPath).string() +
                stdfs::path("/").make_preferred().string(),
            "");
      } else {
        // If it's not empty, we can get the path from its children anyways.
        continue;
      }
    } else {
      if (stdfs::is_regular_file(it)) {
        std::ifstream input(it.path().string(),
                            std::ios::in | std::ios_base::binary);
        std::string buffer;

        input >> buffer;

        items.emplace_back(it.path().lexically_relative(inputData.inputPath),
                           buffer);
      }
    }
  }

  writeConfig(items, inputData, flags, err);

  return err_t::errSuccess;
}
