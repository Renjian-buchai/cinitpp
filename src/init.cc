#include "init.hh"

#include <iostream>

err_t initialise(const std::vector<bool>& flags,
                 const std::filesystem::path& initPath, std::string& err) {
  if (!std::filesystem::is_directory(initPath)) {
    err += "'" + initPath.string() + "' is not a directory. Exiting...\n";
    return err_t::invalidPath;
  }

  if (!flags[flag_t::force]) {
  }

  return err_t::errSuccess;
}