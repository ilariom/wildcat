#ifndef _WKT_COMP_TRAITS_H
#define _WKT_COMP_TRAITS_H

#include <cstdint>

#define REGISTER_COMPONENT(type_name, type_id)      \
    template<>                                      \
    struct comp_traits<type_name>                   \
    {                                               \
        constexpr static int32_t id = type_id;      \
        constexpr static bool is_drawable = false;  \
    }                                              

#define REGISTER_DRAWABLE(type_name, type_id)       \
    template<>                                      \
    struct comp_traits<type_name>                   \
    {                                               \
        constexpr static int32_t id = type_id;      \
        constexpr static bool is_drawable = true;   \
    }

#define REGISTER_SCRIPT(type_name)                                       \
    template<>                                                           \
    struct comp_traits<type_name>                                        \
    {                                                                    \
        constexpr static int32_t id = comp_traits<Script>::type_id;      \
        constexpr static bool is_drawable = false;                       \
    }

namespace wkt {
namespace components
{

template<typename C>
struct comp_traits;

template<typename C>
constexpr int32_t get_type_id()
{
    return comp_traits<C>::id;
}

template<typename C>
constexpr bool is_drawable()
{
    return comp_traits<C>::is_drawable;
}

}}

#endif