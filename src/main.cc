#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "enum.hh"
#include "init.hh"

int main(int argc [[maybe_unused]], char **argv [[maybe_unused]]) {
  namespace stdfs = std::filesystem;
  std::vector<bool> flags(flag_t::flagSize);

  stdfs::path inPath = "";

  std::string err = "";

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      inPath = argv[i];
      continue;
    }

    switch (argv[i][1]) {
    case 'F':
      [[fallthrough]];
    case 'f':
      flags[flag_t::force] = true;
      break;

    case 'I':
      [[fallthrough]];
    case 'i':
      flags[flag_t::input] = true;

      if (++i < argc) {
        inPath = stdfs::path(argv[i]);
      } else {
        std::cout << "No path provided.\n"
                  << "Using path '" + stdfs::current_path().string() + "'.\n";
      }
      break;

    default:
      err += "Invalid argument: '" + std::string(argv[i]) +
             "';\n"
             "Ignoring.\n";
      break;
    }
  }

  if (inPath == "") {
    inPath = stdfs::current_path();
  }

  if (!flags[flag_t::force] && err != "") {
    std::cerr << err + "\n"
              << "If your path starts with '-', please make sure it succeeds "
                 "your invokation of -I.\n";
    return err_t::invalidFlags;
  }

  err_t ret = err_t::errSuccess;

  if (flags[flag_t::input]) {
  } else {
    ret = initialise(flags, inPath, err);
  }

  if (!flags[flag_t::force] && err != "") {
    std::cerr << err << "\n";
  }

  return ret;
}