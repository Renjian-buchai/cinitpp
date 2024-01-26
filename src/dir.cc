#include "../include/dir.hh"

directory::directory(std::filesystem::path path) : _path(path) {}

directory::directory(const directory& dir)
    : _inDir(std::move(dir._inDir)), _path(std::move(dir._path)) {}
