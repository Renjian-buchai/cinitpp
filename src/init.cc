#include "init.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

err_t initialise(const std::vector<bool>& flags,
                 const std::filesystem::path& initPath, std::string& err) {
  namespace stdfs = std::filesystem;

  if (!stdfs::is_directory(initPath) || !stdfs::exists(initPath)) {
    err += "'" + initPath.string() + "' is not a directory. Exiting...\n";
    return err_t::invalidPath;
  }

  if (!flags[flag_t::force] && !stdfs::is_empty(initPath)) {
    err += "'" + initPath.string() +
           "' is not empty.\n"
           "Use `-f` to ignore this warning and proceed anyways.\n";
    return err_t::nonEmptyDir;
  }

  std::vector<stdfs::path> toCreate{"aoeu.txt", "boeu/", "coeu/aoeu.txt"};

  for (const stdfs::path& path : toCreate) {
    char terminating = path.string().back();
    if (terminating == '/' || terminating == '\\') {
      if (!stdfs::exists(path)) {
        stdfs::create_directories(initPath / path);
      }
      continue;
    }

    if (path.has_parent_path() && !stdfs::exists(path.parent_path())) {
      stdfs::create_directories(initPath / path.parent_path());
    }

    std::ofstream((initPath / path).string());
  }

  return err_t::errSuccess;
}