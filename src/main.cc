#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "enum.hh"
#include "init.hh"
#include "input.hh"

#include "util.hh"

int main(int argc, char **argv) {
  namespace stdfs = std::filesystem;
  std::vector<bool> flags(flag_t::flagSize);

  inputData_t inputData;

  std::string err = "";

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      if (inputData.inputPath != "") {
        err += "Candidate input path `" + inputData.inputPath.string() +
               "` succeeded by input path `" + argv[i] + "`.\n";
      }
      inputData.inputPath = argv[i];
      continue;
    }

    switch (argv[i][1]) {
    case 'f':
    forceFlag:
      flags[flag_t::force] = true;
      break;

    case 'I':
    inputFlag:
      flags[flag_t::input] = true;

      if (++i < argc) {
        inputData.inputPath = stdfs::path(argv[i]);
      } else {
        std::cout << "No path provided.\n"
                  << "Using path `" + stdfs::current_path().string() + "`.\n";
      }
      break;

    case 'g':
    globalFlag:
      flags[flag_t::global] = true;
      break;

    case 'c':
    configFlag:
      flags[flag_t::config] = true;

      if (++i < argc) {
        inputData.configIdentifier = argv[i];
      } else {
        std::cout << "No config identifier provided.\n"
                     "Using config `Default`.\n";
      }
      break;

    default:
      std::string option = std::string(argv[i]);

      if (option == "--force") {
        goto forceFlag;
      }

      if (option == "--input") {
        goto inputFlag;
      }

      if (option == "--global") {
        goto globalFlag;
      }

      if (option == "--config") {
        goto configFlag;
      }

      err += "Invalid argument: `" + option +
             "`;\n"
             "Ignoring...\n";
      break;
    }
  }

  if (inputData.inputPath == "") {
    inputData.inputPath = stdfs::current_path();
  }

  if (inputData.inputPath == "") {
    std::cerr << err + "\n"
              << "If your path starts with '-', please make sure it succeeds "
                 "your invokation of -I.\n";
    return err_t::invalidFlags;
  }

  err_t ret = err_t::errSuccess;

  inputData.exePath = stdfs::path(argv[0]).parent_path();

  if (flags[flag_t::input]) {
    ret = autoConf(flags, inputData, err);
  } else {
    ret = initialise(flags, inputData, err);
  }

  if (!flags[flag_t::force] && err != "") {
    std::cerr << err;
  }

  return ret;
}