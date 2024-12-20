#pragma once
#include "inputData.hh"
#if !defined(CINITPP_CFGREADER_HH)
#define CINITPP_CFGREADER_HH

#include "enum.hh"

err_t readConfig(const inputData_t &inputData, const std::vector<bool> flags,
                 dirItems &output, std::string &err);

#endif