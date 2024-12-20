#if !defined(CINITPP_INPUTDATA_HH)
#define CINITPP_INPUTDATA_HH

#include <filesystem>

struct inputData_t {
  std::filesystem::path inputPath = "";
  std::filesystem::path exePath = "";
  std::string configIdentifier = "";
};

#endif