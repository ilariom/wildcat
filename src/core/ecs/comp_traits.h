#ifndef _WKT_COMP_TRAITS_H
#define _WKT_COMP_TRAITS_H

#include <cstdint>

#define REGISTER_COMPONENT(type_name, type_id)      \
    template<>                                      \
    struct comp_traits<type_name>                   \
    {                                               \
        constexpr static int32_t id = type_id;      \
    };                                              \
                                                    \
    template<>                                      \
    struct comp_id_traits<type_id>                  \
    {                                               \
        using type = type_name;                     \
    };                                      

#define REGISTER_DRAWABLE(type_name, type_id)       \
    REGISTER_COMPONENT(type_name, type_id)          \
    template<>                                      \
    struct drawable_traits<type_name>               \
    {                                               \
        constexpr static bool value = true;         \
    };

#define REGISTER_SCRIPT(type_name)                                       \
    template<>                                                           \
    struct comp_traits<type_name>                                        \
    {                                                                    \
        constexpr static int32_t id = comp_traits<Script>::id;           \
    };

namespace wkt {
namespace components
{

template<typename C>
struct comp_traits;

template<int32_t type_id>
struct comp_id_traits;

template<typename C>
struct drawable_traits
{
    constexpr static bool value = false;
};

template<typename C>
constexpr int32_t get_type_id()
{
    return comp_traits<C>::id;
}

template<typename C>
constexpr bool is_drawable()
{
    return drawable_traits<C>::value;
}

}}

#endif