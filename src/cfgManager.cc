#include "cfgManager.hh"

#include <stdlib.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

inline err_t getHomeDir(std::filesystem::path &configPath) {
#if defined(__unix__)
  configPath = std::getenv("HOME");
  if (configPath.empty()) {
    std::cerr << "Unable to find home directory.\n"
                 "Please make sure your HOME environment variable is properly "
                 "configured.\n"
                 "Exiting...";
    return err_t::whereTfIsHome;
  }
#elif defined(_WIN32)
  configPath = std::getenv("USERPROFILE");

  // Fallback in case USERPROFILE is not set.
  if (configPath.empty()) {
    configPath =
        std::string(std::getenv("HOMEDRIVE")) + std::getenv("HOMEPATH");
  }

  if (configPath.empty()) {
    std::cerr << "Unable to find home directory.\n"
                 "We don't know what's happening here, too.\n"
                 "Exiting...\n\n";
    return err_t::whereTfIsHome;
  }
#endif
  return err_t::errSuccess;
}

err_t readConfig(dirItems &output, std::string &err) {
  namespace stdfs = std::filesystem;
  using json = nlohmann::json;

  stdfs::path configPath;

  if (const err_t error = getHomeDir(configPath)) {
    return error;
  }

  if (stdfs::exists(configPath / ".cinitpp.json")) {
    err += "Using user config at '" + (configPath / ".cinitpp.json").string() +
           "'\n";
    configPath /= ".cinitpp.json";
  } else {
    configPath = stdfs::current_path() / ".cinitpp.json";
    if (!stdfs::exists(configPath)) {
      err += "Unable to find '.cinitpp.json' configuration file.\n"
             "Proceeding with cinitpp's default config.\n";

      output = dirItemsDefault;
      return err_t::errSuccess;
    } else {
      err += "Using global config at '" + (configPath).string() + "'\n";
    }
  }

  json::value_type config;

  {
    nlohmann::json configs;
    std::ifstream input(configPath, std::ios::in);
    configs = nlohmann::json::parse(input);

    for (const json::value_type &it : configs) {
      if (!it.is_object()) {
        std::cerr << "Malformed json.\n"
                     "Exiting...\n\n";
        return err_t::malformedJSON;
      }

      if (it.contains("config") && it["config"].is_string() &&
          it["config"].template get<std::string>() == "Default") {
        config = it;
      }
    }

    if (config.empty()) {
      err += "Unable to find 'Default' config.\n"
             "Proceeding with cinitpp's default config.\n";

      output = dirItemsDefault;
      return err_t::errSuccess;
    }

    if (!config.contains("items")) {
      std::cerr << "Malformed json.\n"
                   "Unable to find 'contents' in config: "
                << config["config"]
                << "\n"
                   "Exiting...\n\n";
      return err_t::malformedJSON;
    }

    config = config["items"];
  }

  for (const auto &item : config) {
    if (!item.contains("path")) {
      std::cerr << "Unable to find 'path' in items.\n"
                   "Exiting...\n\n";
      return err_t::malformedJSON;
    }

    if (!item.contains("contents")) {
      std::cerr << "Unable to find 'contents' in directory item: "
                << item["path"]
                << "\n"
                   "Exiting...\n\n";
      return err_t::malformedJSON;
    }

    output.emplace_back(item["path"].template get<std::string>(),
                        item["contents"].template get<std::string>());
  }

  return err_t::errSuccess;
}