#if !defined(CINITPP_INIT_HH)
#define CINITPP_INIT_HH

#include <filesystem>
#include <vector>

#include "enum.hh"

err_t initialise(const std::vector<bool>& flags,
                 const std::filesystem::path& initPath, std::string& errorMsg);

#endif