#pragma once
#if !defined(CINITPP_CFGREADER_HH)
#define CINITPP_CFGREADER_HH

#include "enum.hh"

err_t readConfig(const std::filesystem::path &exePath, dirItems &output,
                 std::string &err);

#endif