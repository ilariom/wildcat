#ifndef json_h
#define json_h

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>

namespace webjson
{
    class Object
    {
    public:
        enum class Type
        {
            MAP,
            VALUE,
            ARRAY
        };

    public:
        Object() = default;
        Object(const Object&);
        Object(Object&&) = default;
        Object& operator=(const Object&);
        Object& operator=(Object&&) = default;
        ~Object() = default;
        
    public:
        
        Object& operator[](const std::string& key);
        Object& operator[](unsigned int index);
        const Object& operator[](const std::string& key) const;
        const Object& operator[](unsigned int index) const;
        inline const Object& at(const std::string& key) const;
        inline const Object& at(unsigned int index) const;
        bool has(const std::string& key) const;
        std::vector<std::string> getKeys() const;
        
        void push(const Object& obj);
        
        void operator=(const std::string& value);
        void operator=(float value);
        
        std::string asString() const;
        float asNumber() const;
        
        bool isMap() const { return type() == Type::MAP; }
        bool isValue() const { return type() == Type::VALUE; }
        bool isArray() const { return type() == Type::ARRAY; }
        bool isEmpty() const { return this->value == "" && this->omap.empty() && this->vect.empty(); }
        bool isNumber() const;
        
        Type type() const { return this->objType; }
        size_t size() const;
        
        std::string toStyledString() const { return toStyledString(""); }
        
        bool operator==(const Object& o) const
        {
            bool isValueEqual = isValue() && this->value == o.value;
            bool isMapEqual = isMap() && this->omap == o.omap;
            bool isArrayEqual = isArray() && this->vect == o.vect;
            
            return type() == o.type()
                && (isValueEqual || isMapEqual || isArrayEqual);
        }
        
    private:
        void setAsMap();
        void setAsValue(const std::string& value);
        void setAsArray();
        
        std::string toStyledString(const std::string& tabs) const;
        
        void setType(Type type) { this->objType = type; }
        
    private:
        
        Type objType = Type::VALUE;
        std::string value;
        std::unordered_map<std::string, std::shared_ptr<Object>> omap;
        std::vector<std::shared_ptr<Object>> vect;
    };
    
    std::string stringify(const Object& json);
    Object parse(const std::string& text, std::string* error = nullptr);
}

#endif /* json_h */
