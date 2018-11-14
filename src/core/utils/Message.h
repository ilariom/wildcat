#ifndef _WKT_MESSAGE_H
#define _WKT_MESSAGE_H

#include "ecs/Entity.h"
#include "ecs/Component.h"
#include <string>
#include <unordered_set>
#include <numeric>

namespace wkt {
namespace utils
{

template<typename T>
class Message
{
private:
    using Entity = wkt::ecs::Entity;
    using EntityId = wkt::ecs::Entity::EntityUniqueID;
    using Component = wkt::ecs::Component;
    using ComponentTypeID = wkt::ecs::Component::ComponentTypeID;

public:
    Message() = default;
    Message(Entity* sender) : sdr(sender) { }

public:
    inline Message& sendTo(EntityId id);
    inline Message& dontSendTo(EntityId id);
    inline Message& andFilter(ComponentTypeID id);
    inline Message& orFilter(ComponentTypeID id);

    void write(T&& content) { this->content = content; }
    const T& read() const { return this->content; }
    const Entity& sender() const { return *this->sdr; }
    inline bool isReceiver(const Entity& rcv) const;

private:
    T content;
    Entity* sdr;
    std::unordered_set<EntityId> sto;
    std::unordered_set<EntityId> dsto;
    std::unordered_set<ComponentTypeID> filtand;
    std::unordered_set<ComponentTypeID> filtor;
};

template<typename T>
inline Message<T>& Message<T>::sendTo(EntityId id)
{ 
    this->sto.insert(id);
    this->dsto.erase(id);
    return *this; 
}

template<typename T>
inline Message<T>& Message<T>::dontSendTo(EntityId id)
{ 
    this->sto.erase(id);
    this->dsto.insert(id);
    return *this; 
}

template<typename T>
inline Message<T>& Message<T>::andFilter(ComponentTypeID id)
{ 
    this->filtand.insert(id);
    return *this; 
}

template<typename T>
inline Message<T>& Message<T>::orFilter(ComponentTypeID id)
{ 
    this->filtor.insert(id);
    return *this; 
}

template<typename T>
inline bool Message<T>::isReceiver(const Entity& rcv) const
{ 
    EntityId eid = rcv.getUID();
    bool inDontSendTo = this->dsto.find(eid) != this->dsto.end();

    if(inDontSendTo)
        return false;

    bool inSendTo = this->sto.find(eid) != this->sto.end();

    bool andFilters = this->filtand.empty() || std::accumulate(this->filtand.begin(), this->filtand.end(), true, 
    [&rcv] (bool inFilters, ComponentTypeID id) {
        return inFilters && !rcv.query(id).empty();
    });

    if(!andFilters)
        return false;

    bool orFilters = this->filtor.empty() || std::accumulate(this->filtor.begin(), this->filtor.end(), false, 
    [&rcv] (bool inFilters, ComponentTypeID id) {
        return inFilters || !rcv.query(id).empty();
    });

    return inSendTo && orFilters;
}

}}

#endif