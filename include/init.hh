#pragma once
#if !defined(CINITPP_INIT_HH)
#define CINITPP_INIT_HH

#include <vector>

#include "enum.hh"
#include "util.hh"

err_t initialise(const std::vector<bool> &flags, const inputData_t &initPath,
                 std::string &errorMsg);

#endif