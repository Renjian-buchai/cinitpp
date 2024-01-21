#include "../include/dir.hh"

directory::directory(directory* prev, std::filesystem::path path)
    : _prev(prev), _path(path) {}

const std::filesystem::path& directory::path() { return _path; }

const directory* directory::prev() { return _prev; }