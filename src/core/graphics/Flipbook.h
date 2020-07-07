#ifndef WKT_FLIPBOOK_H
#define WKT_FLIPBOOK_H

#include "math/wktmath.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace wkt {
namespace gph
{

class Flipbook
{
public:
    struct Card
    {
        Card() = default;

        Card(std::string name, wkt::math::Rect rect, uint16_t times)
            : name(name), rect(rect), times(times)
        { }

        Card(wkt::math::Rect rect, uint16_t times)
            : rect(rect), times(times)
        { }

        Card(wkt::math::Rect rect)
            : rect(rect), times(1)
        { }

        std::string name;
        wkt::math::Rect rect;
        uint16_t times;
    };

    class flipbook_iterator
    {
    public:
        flipbook_iterator(const Flipbook& fb, size_t idx);
        flipbook_iterator(flipbook_iterator&&) = default;

    public:
        const Card& operator*() const;
        flipbook_iterator& operator++();

        bool operator==(const flipbook_iterator& o) const { return this->idx == o.idx; }
        bool operator!=(const flipbook_iterator& o) const { return !(*this == o); }

    private:
        const Flipbook& fb;
        size_t times;
        size_t idx;
    };

    friend flipbook_iterator;

public:
    Flipbook() = default;
    Flipbook(std::vector<Card>&& cards) : cards(cards) { }

public:
    void insert(size_t hint, const Card&);
    void push(const Card&);
    Card top();
    void pop();
    size_t size() const { return this->cards.size(); }

    Card& operator[](const std::string& name);
    const Card& operator[](const std::string& name) const;

    flipbook_iterator begin() const { return flipbook_iterator { *this, 0 }; }
    flipbook_iterator end() const { return flipbook_iterator { *this, size() }; }

private:
    std::vector<Card> cards;
};

class FlipbookChannels
{
public:
    size_t addChannel(const Flipbook&);
    void setChannel(size_t, bool loop = false);

    const Flipbook::Card& next();
    bool hasNext() const;

    size_t getCurrentChannel() const { return this->currentChannel; }
    bool isLooping() const { return this->loop; }

private:
    std::vector<Flipbook> channels;
    std::shared_ptr<Flipbook::flipbook_iterator> it = nullptr;
    size_t currentChannel;
    bool loop;
};

}}

#endif