#include "../include/file.hh"

file::file() : name(""), contents("") {}

file::file(std::filesystem::path _name) : name(_name), contents("") {}

file::file(std::filesystem::path _name, std::string _contents)
    : name(_name), contents(_contents) {}
