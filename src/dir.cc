#include "../include/dir.hh"

directory::directory(directory* prev, std::filesystem::path path)
    : _prev(prev), _path(path) {}

directory::directory(const directory& dir)
    : _inDir(std::move(dir._inDir)),
      _prev(dir._prev),
      _path(std::move(dir._path)) {}

const std::filesystem::path& directory::path() { return _path; }

const directory* directory::prev() { return _prev; }