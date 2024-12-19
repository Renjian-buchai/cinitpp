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

### Initialisation

To initialise, use the following command:

```pwsh
cinitpp
```

To force an initialisation even when the current directory is not empty, use the flag `-F, -f, --force`

Note, the input directory must follow immediately after the flag `-I, -i, --input`.

Using the flag `-G, -g, --global` will read the configuration that's effective globally.

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

To generate a config file, use the flag `-I`

```pwsh
cinitpp -I <INPUT DIRECTORY>
```

The config file will be created at your home directory by default.

To create a config file that's effective globally, include the flag `-G`.

```pwsh
cinitpp -G -I <INPUT DIRECTORY> 
```

Using config file `-F` will suppress all non-error messages (e.g., `Wrote configuration to ~/.cinitpp.json`)

## Limitations

Due to my inability to give a shit and std::filesystem's substandard design choices, Windows configurations will not be compatible with linux.

## Dependencies

- nlohmann/json 3.11.0

## Future plans (So I don't forget)

- Add a way to select the config from the config file
- Add a way to overwrite the config if the name is the same
- ? Use a faster json library
