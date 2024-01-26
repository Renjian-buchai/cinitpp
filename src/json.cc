#include "../include/json.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include "../include/enum.hh"

using json = nlohmann::json;

uint8_t configOut(std::vector<directoryItem>& dirStructure,
                  std::error_code& err) {
  if (std::filesystem::create_directories("../.cinitpp/", err)) {
    return exitVal::dirCreationFailed;
  }

  std::ofstream config("../.cinitpp/config.json");
  if (!config.is_open()) {
    return exitVal::configFileCreationFailed;
  }

  std::string configStr{"{\n"};

  for (auto it = dirStructure.cbegin(); it != dirStructure.cend() - 1; ++it) {
    configStr +=
        "   \"" + it->name.string() + "\": \"" + it->contents + "\",\n";
  }
  configStr += "   \"" + dirStructure.crbegin()->name.string() + "\": \"" +
               dirStructure.crbegin()->contents + "\"\n}";

  // Matches '\\' and replaces with "\\\\". It does not match "RX(\)RX"
  // and replaces it with "\\\\".
  configStr = std::regex_replace(configStr, std::regex(R"RX(\\)RX"), "\\\\");

  config << configStr;
  if (!json::accept(configStr)) {
    std::cerr << "Somehow the config is not valid JSON.\n";
    return exitVal::invalidJSON;
  }

  config.close();
  return exitVal::success;
}
