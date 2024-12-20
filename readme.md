# cinitpp

Initialise your C++ projects easily with cinitpp!

## Currently undergoing a rewrite. Please use branch `remake` instead

## Table of contents

- [cinitpp](#cinitpp)
  - [Currently undergoing a rewrite. Please use branch `remake` instead](#currently-undergoing-a-rewrite-please-use-branch-remake-instead)
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

JSON config format: 

```json
[
  {
    "Path": "./path/to/file.txt", 
    "Contents": "contents"
  }, 
  // ...
]
```

## Usage

To initialise, use the following command: 

```c++
cinitpp
```

To force an initialisation even when the current directory is not empty, use the following command: 

```c++
cinitpp -F
```

To generate a config file, use the following command: 

```c++
cinitpp -I <INPUT DIRECTORY>
```

Note, the directory MUST follow IMMEDIATELY AFTER the flag -I. 

Usage of -I and -F will ignore -F. 

## Future plans

- Making it configurable
- Making it save the configuration

## Build instructions

### Dependencies

- Nlohmann-json 

### Building

You can use the makefile as reference. TLDR:

- Standard = C++17
- Source = src/main.cc main.exe
- Additional include paths = 
- If possible, -Wall -Wextra -Werror (Though I wouldn't mind if there were warnings, I'd still attempt to correct them, so to save time, it'd be best to use all warnings)