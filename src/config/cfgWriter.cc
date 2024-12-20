#include "config/cfgWriter.hh"

#include <fstream>

#include "util.hh"

err_t writeConfig(const dirItems &input, std::string &err,
                  const std::filesystem::path &exePath) {
  namespace stdfs = std::filesystem;
  using json = nlohmann::json;
  stdfs::path configPath;

  if (const err_t error = getHomeDir(configPath)) {
    return error;
  }

  if (exePath != "") {
    configPath = exePath;
  }

  json items = json::array();

  for (const auto &[path, contents] : input) {
    items.push_back(json::object({{"path", path}, {"contents", contents}}));
  }

  json config;

  if (!stdfs::exists(configPath)) {
    config =
        json::array({json::object({{"config", "Default"}, {"items", items}})});
  } else {
    std::ifstream inputStream(configPath / ".cinitpp.json");
    config = json::parse(inputStream);

    config.push_back(json::object({{"config", "aoeu"}, {"items", items}}));
  }

  std::ofstream output(configPath / ".cinitpp.json");
  if (!output.is_open()) {
    std::cerr << "Unable to open or create output file.\n"
                 "Please ensure that `"
              << configPath
              << "` exists, \n"
                 "and cinitpp has the correct permissions create files in `"
              << configPath.string()
              << "`.\n"
                 "Exiting...\n\n";
    return err_t::creationError;
  }
  output << config.dump(2);
  err += "Wrote configuration to `" + (configPath / ".cinitpp.json").string() +
         "`.\n";

  return err_t::errSuccess;
}
