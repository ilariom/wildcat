#ifndef strings_h
#define strings_h

#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

namespace webstrings
{
    
bool is_number(const std::string& s);
std::string hungarian_to_hyphen(const std::string& s);
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);
std::string trim(const std::string& str);

template<typename T>
void to_stringstream(std::stringstream& ss, T t)
{
    ss << t;
}

template<typename T, typename... TS>
void to_stringstream(std::stringstream& ss, T t, TS... ts)
{
    to_stringstream(ss, t);
    to_stringstream(ss, ts...);
}

template<typename... TS>
std::string to_string(TS... ts)
{
    std::stringstream ss;
    to_stringstream(ss, ts...);
    return ss.str();
}

template<typename T>
struct from_string_struct
{
    T operator()(const std::string& str) = delete;
};

template<>
struct from_string_struct<float>
{
    float operator()(const std::string& str)
    {
        return std::strtod(str.c_str(), nullptr);
    }
};

template<>
struct from_string_struct<double>
{
    double operator()(const std::string& str)
    {
        return std::strtod(str.c_str(), nullptr);
    }
};

template<>
struct from_string_struct<int>
{
    int operator()(const std::string& str)
    {
        return std::atoi(str.c_str());
    }
};

template<typename T>
T from_string(const std::string& str)
{
    return from_string_struct<T>()(str);
}

template<typename T>
std::pair<bool, T> safe_from_string(const std::string& str)
{
    std::pair<bool, T> p;
    p.first = is_number(str);
    
    if(p.first)
    {
        p.second = from_string<T>(str);
    }
    
    return p;
}
    
}

#endif /* strings_h */
