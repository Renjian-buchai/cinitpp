# cinitpp

Initialise your C++ projects easily with cinitpp!

## Table of contents

- [cinitpp](#cinitpp)
  - [Table of contents](#table-of-contents)
  - [License](#license)
  - [Documentation](#documentation)
  - [Usage](#usage)
  - [Limitations](#limitations)
  - [Future plans](#future-plans)
  - [Build instructions](#build-instructions)
    - [Dependencies](#dependencies)

## License

Copyright © 2023 Contributors

This project is licensed under the Apache license. View [LICENSE](https://github.com/Renjian-buchai/cinitpp/blob/main/LICENSE) for more details.

## Documentation

JSON config format:

```json
[
  {
    "config": "configName",
    "items": [
      {"name": "path/to/file.txt", "contents": "contents"},

      // Directories will terminate with a '/'; content will be ignored.
      {"name": "path/to/dir/", "contents": ""} 
      
      // Other files and dirs 
    ]
  }

  // Other configs 
]
```

## Usage

To initialise, use the following command:

```pwsh
cinitpp
```

To force an initialisation even when the current directory is not empty, use the following command:

```pwsh
cinitpp -F
```

To generate a config file, use the following command:

```pwsh
cinitpp -I <INPUT DIRECTORY>
```

Note, the directory MUST follow IMMEDIATELY AFTER the flag -I.

Usage of -I and -F will ignore -F.

## Limitations

Due to my inability to give a shit and std::filesystem's default separators, Windows configurations will not be compatible with linux.

## Future plans

- Making it configurable
- Making it save the configuration

## Build instructions

### Dependencies

- nlohmann/json 3.11.0
