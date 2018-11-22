#include "Node.h"
#include <vector>

namespace wkt {
namespace components
{

void Node::visit(std::function<bool(Node&)> fn)
{
    std::vector<Node*> frontier = { this };

    while(!frontier.empty())
    {
        Node* v = frontier.back();
        frontier.pop_back();

        bool continueTraversal = fn(*v);

        if(!continueTraversal || v->prune())
        {
            continue;
        }

        for(auto& spn : v->getChildren())
        {
            frontier.push_back(spn.get());
        }
    }
}

}}