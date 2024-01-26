#include "../include/file.hh"

file::file(const std::filesystem::path path)
    : filePath(path), _contents(std::string{}) {}

file::file(const std::filesystem::path path, const std::string contents)
    : filePath(path), _contents(contents) {}

const std::filesystem::path& file::path() { return filePath; }

const std::string& file::contents() { return _contents; }
