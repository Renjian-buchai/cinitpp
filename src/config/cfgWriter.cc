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

  json items = json::array();

  for (const auto &[path, contents] : input) {
    items.push_back(json::object({{"path", path}, {"contents", contents}}));
  }

  if (exePath == "") {
    json config;

    if (!stdfs::exists(configPath)) {
      config = json::array(
          {json::object({{"config", "Default"}, {"items", items}})});
    } else {
      std::ifstream inputStream(configPath / ".cinitpp.json");
      config = json::parse(inputStream);

      config.push_back(json::object({{"config", "aoeu"}, {"items", items}}));
      std::cout << config.dump(2);
    }

    std::ofstream output(configPath / ".cinitpp.json");
    output << config.dump(2);
    err += "Wrote configuration to '" +
           (configPath / ".cinitpp.json").string() + "'.\n";
    return err_t::errSuccess;
  }

  return err_t::errSuccess;
}
