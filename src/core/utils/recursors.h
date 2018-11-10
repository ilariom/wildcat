#ifndef _WKT_RECURSORS_H
#define _WKT_RECURSORS_H

#include <functional>

namespace wkt {
namespace utils
{

class AbstractRecursor
{
public:
    virtual ~AbstractRecursor() = default;

public:
    virtual void run() = 0;
};

template<typename node_type, typename hnd_ret_type>
class HierarchicalRecursor : public AbstractRecursor
{
public:
    using handler = std::function<hnd_ret_type(node_type&)>;

public:
    virtual ~HierarchicalRecursor() = default;

public:
    void bindRoot(node_type root) { this->root = root; }
    void bindHandler(handler hnd) { this->hnd = hnd; }

protected:
    node_type& getRoot() { return this->root; }
    handler& getHandler() { return this->hnd; }

private:
    node_type root;
    handler hnd;
};

template<typename obj_type, typename iter_type, typename hnd_ret_type>
class SequentialRecursor : public AbstractRecursor
{
public:
    using iterator = iter_type;
    using handler = std::function<hnd_ret_type(obj_type)>;

public:
    virtual ~SequentialRecursor() = default;

public:
    inline void bindIterators(iter_type begin, iter_type end);
    void bindHandler(handler hnd) { this->hnd = hnd; }

protected:
    iter_type begin() { return this->b; }
    iter_type end() { return this->e; }
    handler& getHandler() { return this->hnd; }

private:
    iter_type b, e;
    handler hnd;
};

template<typename obj_type, typename iter_type, typename hnd_ret_type>
inline void SequentialRecursor<obj_type, iter_type, hnd_ret_type>::bindIterators(iter_type begin, iter_type end)
{
    this->b = begin;
    this->e = end;
}

}}

#endif