#include "../include/json.hh"

#include <filesystem>
#include <fstream>
#include <iostream>

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

  if (!json::accept(configStr)) {
    std::cerr << "Somehow the config is not valid JSON.\n";
    return exitVal::invalidJSON;
  }

  config << configStr;
  config.close();
  return exitVal::success;
}
