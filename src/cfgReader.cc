#include "cfgReader.hh"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "rapidjson.hh"

err_t readConfig(dirItems& output, std::string& err) {
  namespace stdfs = std::filesystem;
  namespace json = rapidjson;

  stdfs::path configPath;

  if (stdfs::exists(stdfs::current_path() / ".cinitpp.json")) {
    configPath = stdfs::current_path() / ".cinitpp.json";
  } else {
#if defined(__unix__)
    configPath = std::getenv("HOME");
    if (configPath.empty()) {
      err +=
          "Unable to find home directory.\n"
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
      err +=
          "Unable to find home directory.\n"
          "We don't know what's happening here, too.\n"
          "Exiting...";
      return err_t::whereTfIsHome;
    }
#endif

    configPath /= ".cinitpp.json";
  }

  std::string data;
  {
    std::ifstream input(configPath, std::ios::in);
    std::string buffer;
    while (std::getline(input, buffer)) {
      data += buffer + "\n";
    }
  }

  json::Value contents(json::kArrayType);

  {
    json::Value::Array configs =
        json::Document().Parse(data.c_str()).GetArray();

    for (auto it = configs.Begin(); it != configs.End(); ++it) {
      std::cout << it->GetType();
      if (!it->HasMember("config")) {
        err +=
            "Unable to get config name of an object.\n"
            "Ignoring...\n";
        // continue;
      }

      if ((*it)["config"] == "Default") {
        if (!it->HasMember("contents")) {
          err +=
              "Unable to get contents of folder to be initialised.\n"
              "Exiting...";
          return err_t::missingContents;
        }

        contents = (*it)["contents"].GetArray();
        break;
      }
    }
  }

  output.clear();

  std::cout << err;

  for (auto& item : contents.GetArray()) {
    output.emplace_back(item["name"].GetString(), item["contents"].GetString());
  }

  return err_t::errSuccess;
}