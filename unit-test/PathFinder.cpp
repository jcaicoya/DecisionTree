#include "PathFinder.h"

std::map<std::string, std::optional<std::filesystem::path>> PathFinder::_nameToPath;


std::optional<std::filesystem::path> PathFinder::Find(const std::string name)
{
     const auto itr = _nameToPath.find(name);
     if (_nameToPath.end() != itr)
     {
         return itr->second;
     }
     std::filesystem::path from = std::filesystem::current_path();
     std::optional<std::filesystem::path> foundPath = FindPath(name, from);
     if (foundPath.has_value())
     {
         _nameToPath[name] = foundPath;
         return foundPath;
     }
     if (from.has_parent_path())
     {
         from = from.parent_path().parent_path();
         foundPath = FindPath(name, from);
     }
      
     _nameToPath[name] = foundPath;
     return foundPath;
}


std::optional<std::filesystem::path> PathFinder::FindPath(const std::string &fileOrDirectory, const std::filesystem::path &from)
{
    for (const auto &entry : std::filesystem::recursive_directory_iterator(from))
    {
        const auto current = entry.path().filename().string();
        if (fileOrDirectory == current)
        {
            return entry.path();
        }
    }

    return std::optional<std::filesystem::path>();
}