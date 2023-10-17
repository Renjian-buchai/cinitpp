#include "../include/directoryItem.hh"

directoryItem::directoryItem() : name(""), contents("") {}

directoryItem::directoryItem(std::filesystem::path _name)
    : name(_name), contents("") {}

directoryItem::directoryItem(std::filesystem::path _name, std::string _contents)
    : name(_name), contents(_contents) {}
