#include "init.hh"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config/cfgReader.hh"
#include "inputData.hh"

err_t initialise(const std::vector<bool> &flags, const inputData_t &inputData,
                 std::string &err) {
  namespace stdfs = std::filesystem;

  if (!stdfs::is_directory(inputData.inputPath) ||
      !stdfs::exists(inputData.inputPath)) {
    std::cerr << "`" + inputData.inputPath.string() +
                     "` is not a directory.\n"
                     "Exiting...\n\n";
    return err_t::invalidPath;
  }

  if (!flags[flag_t::force] && !stdfs::is_empty(inputData.inputPath)) {
    err += "`" + inputData.inputPath.string() +
           "` is not empty.\n"
           "Use `-f` to ignore this warning and proceed anyways.\n";
    return err_t::nonEmptyDir;
  }

  dirItems toCreate{};
  if (const err_t error = readConfig(inputData, flags, toCreate, err)) {
    return error;
  }

  for (const auto &[path, content] : toCreate) {
    std::string pathstr = path.string();
    if (pathstr.back() == '/' ||
        pathstr.substr(pathstr.size() - 2) == R"(\\)") {
      if (!stdfs::exists(path)) {
        stdfs::create_directories(inputData.inputPath / path);
      }
      continue;
    }

    if (path.has_parent_path() && !stdfs::exists(path.parent_path())) {
      stdfs::create_directories(inputData.inputPath / path.parent_path());
    }

    {
      std::ofstream output((inputData.inputPath / path).string(),
                           std::ios_base::out | std::ios_base::binary);
      output << content;
    }
  }

  return err_t::errSuccess;
}