# cinitpp

Initialise your C++ projects easily with cinitpp!

## Table of contents

- [cinitpp](#cinitpp)
  - [Table of contents](#table-of-contents)
  - [License](#license)
  - [Documentation](#documentation)
  - [Usage](#usage)
    - [Initialisation](#initialisation)
    - [Configuration](#configuration)
  - [Limitations](#limitations)
  - [Dependencies](#dependencies)
  - [Future plans (So I don't forget)](#future-plans-so-i-dont-forget)

## License

Copyright © 2023-2024 Contributors

This project is licensed under the Apache license. View [LICENSE](https://github.com/Renjian-buchai/cinitpp/blob/main/LICENSE) for more details.

## Documentation

JSON config format:

```json
[
  {
    "config": "configName",
    "items": [
      {"path": "path/to/file.txt", "contents": "contents"},

      // Directories will terminate with a '/'; content will be ignored.
      {"path": "path/to/dir/", "contents": ""} 
      
      // Other files and dirs 
    ]
  }

  // Other configs 
]
```

## Usage

```pwsh
cinitpp [-I | --input] <path> [-g | --global] [-f | --force] [-c | --config [identifier]]
```

### Initialisation

To initialise, use the following command:

```pwsh
cinitpp
```

To force an initialisation even when the current directory is not empty, use the flag `-f, --force`

Using the flag `-c, --config` will read the specific configuration within the config file.

```pwsh
cinitpp -c <CONFIG IDENTIFIER>
```

Note, the config name must follow immediately after the flag. When not specified, the configuration `Default` will be read from.

Using the flag `-g, --global` will read the configuration that's effective globally.

```pwsh
cinitpp -G
```

By default, cinitpp will search for the local config, (i.e., the configuration located in the home directory) followed by the global config. If none of those are available, it will use the default initialisation data:

```pwsh
./
├───include/
├───src/
│   └───main.cpp 
│       "int main() {}"
│
├───README.md
│   ""
└───CMakeLists.txt
    ""
```

### Configuration

To generate a config file, use the flag `-I, --input`

```pwsh
cinitpp -I <INPUT DIRECTORY>
```

Note, the input directory must follow immediately after the flag `-I, --input`.

The config file will be created at your home directory by default.

To create a config file that's effective globally, include the flag `-g, --global`.

```pwsh
cinitpp -G -I <INPUT DIRECTORY> 
```

To add a configuration with a unique identifier to the config file, use the flag `-c, --config`. If it already exists, the previous configuration associated with that identifier will be overwritten.

```pwsh
cinitpp -I <INPUT DIRECTORY> -c <CONFIG IDENTIFIER>
```

If a configuration identifier is not provided, the configuration `Default` will be created or overwritten.

Using config file `-f, --force` will suppress all non-error messages (e.g., `Wrote configuration to ~/.cinitpp.json`)

## Limitations

Due to my inability to give a shit and std::filesystem's substandard design choices, Windows configurations will not be compatible with linux.

## Dependencies

- nlohmann/json 3.11.0

## Future plans (So I don't forget)

- Add help menu
- Add a way to select the config from the config file
- Add a way to overwrite the config if the name is the same
- ? Use a faster json library
