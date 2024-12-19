#pragma once
#if !defined(CINITPP_INPUT_HH)
#define CINITPP_INPUT_HH

#include "enum.hh"
#include "inputData.hh"

err_t autoConf(const std::vector<bool> &flags, const inputData_t &data,
               std::string &err);

#endif