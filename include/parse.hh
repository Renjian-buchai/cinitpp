#if !defined(PARSE_HH_)
#define PARSE_HH_

#include <filesystem>
#include <system_error>
#include <vector>

#include "../include/directoryItem.hh"

int ParseDir(std::filesystem::path dirToParse,
             std::vector<directoryItem>& target, std::error_code& err);
int ParseItem(std::filesystem::path, std::filesystem::path,
              std::vector<directoryItem>&, std::error_code&);

#endif  // PARSE_HH_
