#if !defined(JSON_HH_)
#define JSON_HH_

#include <cstdint>
#include <vector>

#include "../include/directoryItem.hh"
#include "../include/external/nlohmann/json.hpp"

using json = nlohmann::json;

uint8_t configOut(std::vector<directoryItem>& dirStructure,
                  std::error_code& err);

#endif  // JSON_HH_
