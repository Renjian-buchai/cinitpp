#include "../include/errHandler.hh"

#include <filesystem>
#include <iostream>

#include "../include/enum.hh"

void reset(uint8_t errNumber, std::error_code& err) {
  if (errNumber) {
    for (auto& dirEntry : std::filesystem::recursive_directory_iterator(".")) {
      if (!std::filesystem::remove_all(dirEntry.path(), err)) {
        std::cerr << err.message();
        std::exit(exitVal::deletionFailed);
      }
    }
    std::exit(errNumber);
  }
}
