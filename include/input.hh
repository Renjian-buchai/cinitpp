#pragma once
#if !defined(CINITPP_INPUT_HH)
#define CINITPP_INPUT_HH

#include "enum.hh"

err_t autoConf(const std::vector<bool> &flags,
               const std::filesystem::path &readPath, std::string &err,
               const std::filesystem::path &exePath);

#endif