#ifndef _WKT_DICTIONARY_H
#define _WKT_DICTIONARY_H

#include "ecs/Component.h"
#include <unordered_map>
#include <memory>

namespace wkt {
namespace components
{

class AbstractDictionary : public wkt::ecs::Component
{
public:
    virtual ~AbstractDictionary() = default;

public:
    bool unique() const override { return false; }
};

template<typename key_type, typename mapped_type, typename key_hasher = std::hash<key_type>>
class Dictionary : public AbstractDictionary, public std::unordered_map<key_type, mapped_type, key_hasher>
{ };

template<typename key_type, typename mapped_type, typename key_hasher>
Dictionary<key_type, mapped_type, key_hasher>& getDictionary(AbstractDictionary& ad)
{
    return *static_cast<Dictionary<key_type, mapped_type, key_hasher>*>(&ad);
}

template<typename key_type, typename mapped_type, typename key_hasher>
std::shared_ptr<AbstractDictionary> make_abstract_dictionary_from(const Dictionary<key_type, mapped_type, key_hasher>& dict)
{
    return std::static_pointer_cast<AbstractDictionary>(std::make_shared<Dictionary<key_type, mapped_type, key_hasher>>(dict));
}

template<typename key_type, typename mapped_type, typename key_hasher = std::hash<key_type>>
std::shared_ptr<Dictionary<key_type, mapped_type, key_hasher>> get_dictionary(const std::shared_ptr<AbstractDictionary>& ad)
{
    return std::static_pointer_cast<Dictionary<key_type, mapped_type, key_hasher>(ad);
}



REGISTER_COMPONENT(AbstractDictionary, -6);

}}

#endif