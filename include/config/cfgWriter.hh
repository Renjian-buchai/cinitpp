#pragma once
#if !defined(CINITPP_CFGWRITER_HH)

#include "enum.hh"

err_t writeConfig(const dirItems &input, std::string &err,
                  const std::filesystem::path &exePath = "");

#endif