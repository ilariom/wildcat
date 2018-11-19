#include "webstrings.h"
#include <algorithm>

namespace webstrings
{
    
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> splitted;
    
    auto start = 0U;
    auto end = str.find(delimiter);
    
    while(end != std::string::npos)
    {
        splitted.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    if(splitted.size() == 0)
    {
        splitted.push_back(str);
    }
    
    return splitted;
}

std::string trim(const std::string& str)
{
    std::string t = str;
    
    t.erase(
            t.begin(),
            std::find_if(t.begin(),
                         t.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))
                         )
            );
    
    t.erase(
            std::find_if(t.rbegin(),
                         t.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))
                         ).base(),
            t.end()
            );
    
    return t;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it;
    
    if(*(s.begin()) == '-')
    {
        it = s.begin() + 1;
    }
    else
    {
        it = s.begin();
    }
    
    bool dotVisited = false;
    
    while (it != s.end() && (std::isdigit(*it) || (*it == '.' && !dotVisited)))
    {
        if (*it == '.')
        {
            dotVisited = true;
        }
        
        ++it;
    }
    return !s.empty() && it == s.end();
}

std::string hungarian_to_hyphen(const std::string& s)
{
    if (s.size() < 1)
    {
        return "";
    }
    
    std::string result = "";
    result += std::tolower(s[0]);
    
    for (unsigned int i = 1; i < s.size(); i++)
    {
        if (std::isupper(s[i]))
        {
            result += "-";
            result += std::tolower(s[i]);
            continue;
        }
        
        result += s[i];
    }
    
    return result;
}
    
}
