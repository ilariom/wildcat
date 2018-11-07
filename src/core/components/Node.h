#ifndef _SKR_NODE_H
#define _SKR_NODE_H

#include "ecs/Component.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace wkt {
namespace components
{

class Node : public wkt::ecs::Component
{
    using Component = wkt::ecs::Component;

public:

    inline void appendChild(std::shared_ptr<Node> node);
    inline void removeChild(std::shared_ptr<Node> node);
    void removeAllChildren() { this->children.clear(); }
    inline void removeFromParent();
    const Node* getParent() const { return this->parent; }
    bool hasParent() const { return getParent() != nullptr; }

    const std::vector<std::shared_ptr<Node>>& getChildren() const { return this->children; }

    void visit(std::function<bool(Node&)> fn);

private:
    std::vector<std::shared_ptr<Node>> children;
    Node* parent;
};

inline void Node::appendChild(std::shared_ptr<Node> node)
{
    this->children.push_back(node);
    node->parent = this;
}

inline void Node::removeChild(std::shared_ptr<Node> node)
{
    auto it = std::find(this->children.begin(), this->children.end(), node);

    if(it != this->children.end())
    {
        (*it)->parent = nullptr;
        this->children.erase(it);
    }
}

inline void Node::removeFromParent()
{
    auto par = this->parent;
    const auto& children = par->getChildren();
    std::shared_ptr<Node> self = nullptr;

    for(auto& ptr : children)
    {
        if(*ptr == *this)
        {
            self = ptr;
            break;
        }
    }

    if(self)
    {
        par->removeChild(self);
    }
}

REGISTER_COMPONENT(Node, -1);

}} // wkt

#endif