#include "util.hh"

err_t getHomeDir(std::filesystem::path &configPath) {
#if defined(__unix__)
  const char *homePath = std::getenv("HOME");
  if (homePath == nullptr) {
    configPath = "";
  }

  if (configPath.empty()) {
    std::cerr << "Unable to find home directory.\n"
                 "Please make sure your HOME environment variable is properly "
                 "configured.\n"
                 "Exiting...";
    return err_t::whereTfIsHome;
  }
#elif defined(_WIN32)
  configPath = std::string(std::getenv("USERPROFILE"));

  // Fallback in case USERPROFILE is not set.
  if (configPath.empty()) {
    const char *homeDrive = std::getenv("HOMEDRIVE");
    if (homeDrive == nullptr) {
      homeDrive = "";
    }

    const char *homePath = std::getenv("HOMEPATH");
    if (homePath == nullptr) {
      homePath = "";
    }

    configPath = std::string(homeDrive) + homePath;
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
