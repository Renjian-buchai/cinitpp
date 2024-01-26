#include "../include/file.hh"

file::file(const std::filesystem::path path)
    : filePath(path), _contents(std::string{}) {}

file::file(const std::filesystem::path path, const std::string contents)
    : filePath(path), _contents(contents) {}
