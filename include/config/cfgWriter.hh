#pragma once
#if !defined(CINITPP_CFGWRITER_HH)

#include "enum.hh"
#include "util.hh"

err_t writeConfig(const dirItems &input, const inputData_t &inputData,
                  const std::vector<bool> &flags, std::string &err);

#endif