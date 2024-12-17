#include "init.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using dirItems = std::vector<std::pair<std::filesystem::path, std::string>>;

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

  dirItems toCreate{
      {"aoeu.txt", "aoeu\nboeu\n"}, {"boeu/", ""}, {"coeu/aoeu.txt", ""}};

  for (const auto& [path, content] : toCreate) {
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

    {
      std::ofstream output((initPath / path).string(),
                           std::ios_base::out | std::ios_base::binary);
      output << content;
    }
  }

  return err_t::errSuccess;
}