#include "util.hh"

err_t getHomeDir(std::filesystem::path &configPath) {
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
