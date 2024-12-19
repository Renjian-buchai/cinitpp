#include "init.hh"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "cfgManager.hh"

err_t initialise(const std::filesystem::path &&exePath,
                 const std::vector<bool> &flags,
                 const std::filesystem::path &initPath, std::string &err) {
  namespace stdfs = std::filesystem;

  if (!stdfs::is_directory(initPath) || !stdfs::exists(initPath)) {
    err += "'" + initPath.string() +
           "' is not a directory.\n"
           "Exiting...\n";
    return err_t::invalidPath;
  }

  if (!flags[flag_t::force] && !stdfs::is_empty(initPath)) {
    err += "'" + initPath.string() +
           "' is not empty.\n"
           "Use `-f` to ignore this warning and proceed anyways.\n";
    return err_t::nonEmptyDir;
  }

  dirItems toCreate{};
  readConfig(toCreate, err);

  for (const auto &[path, content] : toCreate) {
    std::string pathstr = path.string();
    if (pathstr.back() == '/' ||
        pathstr.substr(pathstr.size() - 2) == R"(\\)") {
      if (!stdfs::exists(path)) {
        stdfs::create_directories(initPath / path);
      }
      continue;
    }

    if (path.has_parent_path() && !stdfs::exists(path.parent_path())) {
      stdfs::create_directories(initPath / path.parent_path());
    }

    {
      std::ofstream output((initPath / path).string(),
                           std::ios_base::out | std::ios_base::binary);
      output << content;
    }
  }

  return err_t::errSuccess;
}