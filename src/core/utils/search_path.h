#ifndef WKT_SEARCH_PATH_H
#define WKT_SEARCH_PATH_H

#include <string>

namespace wkt {
namespace path
{

void add(const std::string& path);
std::string locate(const std::string& filename);

}}

#endif