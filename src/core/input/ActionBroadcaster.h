#ifndef WKT_ACTION_BROADCASTER_H
#define WKT_ACTION_BROADCASTER_H

#include "components/ActionReceiver.h"
#include <vector>

namespace wkt {
namespace events
{

class ActionBroadcaster
{
    using Action = wkt::components::Action;
public:
    void postAction(Action action) { this->actions.push_back(std::move(action)); }
    std::vector<Action>&& takeActions() { return std::move(this->actions); }
    void close() { this->actions.clear(); }

private:
    std::vector<Action> actions;
};

}}

#endif