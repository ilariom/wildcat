#include "search_path.h"
#include <unordered_map>
#include <vector>
#include <cstdio>

namespace {

std::unordered_map<std::string, std::string> paths;
std::vector<std::string> searchPaths;

}

namespace wkt {
namespace path
{

void add(const std::string& path)
{
    searchPaths.push_back(path);
}

std::string locate(const std::string& filename)
{
    if (paths.find(filename) == paths.end())
    {
        for (const std::string& basepath : searchPaths)
        {
            std::string fullpath = basepath + "/" + filename;

            if (FILE* f = fopen(fullpath.c_str(), "r"))
            {
                fclose(f);
                paths[filename] = fullpath;
                break;
            }
        }
    }

    return paths[filename];
}

}}