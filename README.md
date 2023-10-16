# Cinitpp

Initialise your C++ projects easily with cinitpp!

## Table of contents

- [Cinitpp](#cinitpp)
  - [Table of contents](#table-of-contents)
  - [License](#license)
  - [Documentation](#documentation)
  - [Usage](#usage)
  - [Future plans](#future-plans)
  - [Build instructions](#build-instructions)
    - [Dependencies](#dependencies)
    - [Building](#building)

## License

Copyright © 2023 Contributors

This project is licensed under the Apache license. View [LICENSE](https://github.com/Renjian-buchai/cinitpp/blob/main/LICENSE) for more details.

## Documentation

None for now.

## Usage

```c++
cinitpp
```

## Future plans

- Making it configurable
- Making it save the configuration

## Build instructions

### Dependencies

- [nlohmann/json](https://github.com/nlohmann/json).
  - Version 3.11.2-1
  - Included in [include/external/](https://github.com/Renjian-buchai/cinitpp/blob/main/include/external)
    <!-- As of the branch, does not work. Wait for it to be merged sooner or later.-->

### Building

You can use the makefile as reference. TLDR:

- Standard = C++17
- Source = src/main.cc main.exe
- If possible, -Wall -Wextra -Werror (Though I wouldn't mind if there were warnings, I'd still attempt to correct them, so to save time, it'd be best to use all warnings)
