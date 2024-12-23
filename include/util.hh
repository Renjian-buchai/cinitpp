#pragma once
#if !defined(CINITPP_UTIL_CC)
#define CINITPP_UTIL_CC

#include "enum.hh"

err_t getHomeDir(std::filesystem::path &configPath);

struct inputData_t {
  std::filesystem::path inputPath = "";
  std::filesystem::path exePath = "";
  std::string configIdentifier = "Default";
};

#endif
