#include <cstdint>
#include <string>

enum class flag_t : uint8_t {
  // Input run, i.e. don't run cinitpp
  input,
  // Force initialise, even if dir is not empty
  force
};

uint8_t flipTrue(uint8_t& flags, flag_t flag) {
  return flags | (1 << static_cast<uint8_t>(flag));
}

uint8_t flipFalse(uint8_t& flags, flag_t flag) {
  return flags & ~(1 << static_cast<uint8_t>(flag));
}

uint8_t checkTrue(uint8_t flags, flag_t flag) {
  return flags & (1 << static_cast<uint8_t>(flag));
}

int main(int argc, const char** argv) {
  (void)argc, (void)argv;

  std::string buffer;

  uint8_t flags;

  switch (argc) {
    case 1:
      break;

    default:
      for (int i = 1; i < argc; ++i) {
        if (argv[i] == "-I") {
          buffer = argv[++i];
          flipTrue(flags, flag_t::input);
        }

        else if (argv[i] == "-F") {
          flipTrue(flags, flag_t::force);
        }
      }
  }

  if (checkTrue(flags, flag_t::input)) {
    // validate buffer, then somehow turn it into a vector
  }

  if (!checkTrue(flags, flag_t::force)) {
    // Check if dir is empty
  }

  return 0;
}
