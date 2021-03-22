#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <optional>


class PathFinder
{
public:
    static std::optional<std::filesystem::path> Find(const std::string name);

private:
    static std::map<std::string, std::optional<std::filesystem::path>> _nameToPath;
    static std::optional<std::filesystem::path> FindPath(const std::string &name, const std::filesystem::path &from = std::filesystem::current_path());
};