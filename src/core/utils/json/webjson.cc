#include "webjson.h"
#include "gason.h"
#include "webstrings.h"

#include <cstring>
#include <functional>

namespace webjson
{
    
namespace
{
    Object buildObjects(JsonValue& jsonValue)
    {
        Object obj;
        
        switch(jsonValue.getTag())
        {
            case JSON_NUMBER:
                obj = jsonValue.toNumber();
                break;
                
            case JSON_STRING:
                obj = jsonValue.toString();
                break;
                
            case JSON_ARRAY:
            {
                int k = 0;
                
                for(auto i : jsonValue)
                {
                    obj[k++] = buildObjects(i->value);
                }
                
                break;
            }
                
            case JSON_OBJECT:
            {
                for(auto i : jsonValue)
                {
                    obj[i->key] = buildObjects(i->value);
                }
                
                break;
            }
                
            case JSON_TRUE:
                obj = "true";
                break;
                
            case JSON_FALSE:
                obj = "false";
                break;
                
            default:
            case JSON_NULL:
                break;
        }
        
        return obj;
    };
}

Object parse(const std::string& text, std::string* error /* = nullptr */)
{
    int N = static_cast<int>(text.size());
    char source[N + 1];
    strncpy(source, text.c_str(), N);
    source[N] = '\0';
    char* endptr;
    JsonValue value;
    JsonAllocator allocator;
    int status = jsonParse(source, &endptr, &value, allocator);
    
    if(status != JSON_OK)
    {
        if (error)
        {
            *error = jsonStrError(status);
        }
        
        return Object();
    }
    
    return buildObjects(value);
}

std::string stringify(const Object& json)
{
    return json.toStyledString();
}

Object::Object(const Object& other)
{
    this->objType = other.objType;
    this->value = other.value;

    for(auto p : other.omap)
        this->omap[p.first] = std::make_shared<Object>(*p.second);

    for(auto o : other.vect)
        this->vect.push_back(std::make_shared<Object>(*o));
}

Object& Object::operator=(const Object& other)
{
    this->objType = other.objType;
    this->value = other.value;

    for(auto p : other.omap)
        this->omap[p.first] = std::make_shared<Object>(*p.second);

    for(auto o : other.vect)
        this->vect.push_back(std::make_shared<Object>(*o));

    return *this;
}

Object& Object::operator[](const std::string& key)
{
    setAsMap();
    
    if(this->omap.find(key) == this->omap.end())
    {
        this->omap[key] = std::make_shared<Object>();
    }
    
    return *this->omap[key];
}

Object& Object::operator[](unsigned int index)
{
    setAsArray();
    
    while(index >= this->vect.size())
    {
        this->vect.push_back(std::make_shared<Object>());
    }
    
    return *this->vect[index];
}

const Object& Object::operator[](const std::string& key) const
{
    return this->at(key);
}

const Object& Object::operator[](unsigned int index) const
{
    return this->at(index);
}

const Object& Object::at(const std::string& key) const
{
    Object& self = *(const_cast<Object*>(this));
    return static_cast<const Object&>(self[key]);
}

const Object& Object::at(unsigned int index) const
{
    Object& self = *(const_cast<Object*>(this));
    return static_cast<const Object&>(self[index]);
}

bool Object::has(const std::string& key) const
{
    if (!isMap())
    {
        return false;
    }
    
    return this->omap.find(key) != this->omap.end();
}

std::vector<std::string> Object::getKeys() const
{
    if (!isMap())
    {
        return {};
    }
    
    std::vector<std::string> keys = {};
    
    for (auto it = this->omap.cbegin(); it != this->omap.cend(); it++)
    {
        keys.push_back(it->first);
    }
    
    return keys;
}

void Object::push(const Object& obj)
{
    setAsArray();
    this->vect.push_back(std::make_shared<Object>(obj));
}

void Object::operator=(const std::string& value)
{
    setAsValue(value);
}

void Object::operator=(float value)
{
    setAsValue(webstrings::to_string(value));
}

std::string Object::asString() const
{
    if(!isValue())
    {
        return "";
    }
    
    return this->value;
}

float Object::asNumber() const
{
    if(!(isValue() && isNumber()))
    {
        return 0;
    }
    
    return webstrings::from_string<float>(this->value);
}

void Object::setAsMap()
{
    if(isMap())
    {
        return;
    }
    
    if(isArray())
    {
        this->vect.clear();
    }
    
    setType(Type::MAP);
    this->value = "";
}

void Object::setAsValue(const std::string& value)
{
    if(isMap())
    {
        this->omap.clear();
    }
    
    if(isArray())
    {
        this->vect.clear();
    }
    
    setType(Type::VALUE);
    this->value = value;
}

void Object::setAsArray()
{
    if(isArray())
    {
        return;
    }
    
    if(isMap())
    {
        this->omap.clear();
    }
    
    setType(Type::ARRAY);
    this->value = "";
}

bool Object::isNumber() const
{
    return webstrings::is_number(this->value);
}

size_t Object::size() const
{
    if(isEmpty())
    {
        return 0;
    }
    
    switch(type())
    {
        case Type::VALUE:
            return 1;
        case Type::ARRAY:
            return this->vect.size();
        case Type::MAP:
            return this->omap.size();
        default:
            return 0;
    }
}

std::string Object::toStyledString(const std::string& tabs) const
{
    const std::string emptyObject = "{}";
    const std::string emptyArray = "[]";
    
    if(this->isEmpty())
    {
        return isArray() ? emptyArray : emptyObject;
    }
    
    if(isValue())
    {
        return std::move(asString());
    }
    
    auto wrapInQuotes = [] (const std::string& str) -> std::string {
        std::string res;
        res += "\"";
        res += str;
        res += "\"";
        
        return res;
    };
    
    auto addTabs = [&tabs] (const std::string& str) -> std::string {
        std::string res = tabs + "\t";
        res += str;
        return res;
    };
    
    auto buildRecursively = [&tabs, wrapInQuotes] (const Object& value, std::function<std::string(const std::string&)> addTabs = nullptr) {
        addTabs = addTabs ? addTabs : [] (const std::string& s) { return s; };
        std::string res;
        
        if(value.isValue())
        {
            res = value.isNumber() ? value.asString() : wrapInQuotes(value.asString());
        }
        else
        {
            res = std::move(value.toStyledString(tabs + "\t"));
        }
        
        return addTabs(res);
    };
    
    if(isMap())
    {
        std::string res = "{\n";
        
        for(auto it = this->omap.begin(); it != this->omap.end(); ++it)
        {
            if(it != this->omap.begin())
            {
                res += ",\n";
            }
            
            const std::string& key = it->first;
            const Object& value = *it->second;
            
            res += addTabs(wrapInQuotes(key));
            res += ": ";
            res += buildRecursively(value);
        }
        
        res += "\n" + tabs + "}";
        
        return res;
    }
    
    if(isArray())
    {
        std::string res = "[\n";
        
        for(auto it = this->vect.begin(); it != this->vect.end(); ++it)
        {
            if(it != this->vect.begin())
            {
                res += ",\n";
            }
            
            const Object& value = **it;
            res += buildRecursively(value, addTabs);
        }
        
        res += "\n" + tabs + "]";
        
        return res;
    }
    
    return emptyObject;
}
    
} // namespace json
