#include "Flipbook.h"
#include <algorithm>

namespace wkt {
namespace gph
{

void Flipbook::insert(size_t hint, const Card& card)
{
    this->cards.insert(this->cards.begin() + hint, card);
}

void Flipbook::push(const Card& card)
{
    this->cards.push_back(card);
}

Flipbook::Card Flipbook::top()
{
    return this->cards.back();
}

void Flipbook::pop()
{
    this->cards.pop_back();
}

Flipbook::Card& Flipbook::operator[](const std::string& name)
{
    return *const_cast<Flipbook::Card*>(
        &((*this)[name])
    );
}

const Flipbook::Card& Flipbook::operator[](const std::string& name) const
{
    auto it = std::find_if(this->cards.begin(), this->cards.end(), [&name] (const Card& card) {
        return card.name == name;
    });

    return *it;
}

Flipbook::flipbook_iterator::flipbook_iterator(const Flipbook& fb, size_t idx)
    : fb(fb), idx(idx)
{
    if (fb.size() > idx)
        this->times = fb.cards[idx].times;
}

const Flipbook::Card& Flipbook::flipbook_iterator::operator*() const
{
    return this->fb.cards[this->idx];
}

Flipbook::flipbook_iterator& Flipbook::flipbook_iterator::operator++()
{
    if (this->idx >= this->fb.size())
        return *this;
    
    if (this->times == 1 && this->idx + 1 < this->fb.size())
    {
        this->idx++;
        this->times = this->fb.cards[this->idx].times;
    }
    else if (this->times > 0)
        this->times--;
    else
        this->idx = this->fb.size();

    return *this;
}

size_t FlipbookChannels::addChannel(const Flipbook& fb)
{
    this->channels.push_back(fb);
    return this->channels.size() - 1;
}

void FlipbookChannels::setChannel(size_t channel, bool loop)
{
    this->it = std::make_shared<Flipbook::flipbook_iterator>(
        this->channels[channel].begin()
    );

    this->currentChannel = channel;
    this->loop = loop;
}

const Flipbook::Card& FlipbookChannels::next()
{
    if (this->loop && *this->it == this->channels[this->currentChannel].end())
    {
        setChannel(this->currentChannel, this->loop);
    }

    const Flipbook::Card& r = **this->it;
    ++*this->it;
    return r;
}

bool FlipbookChannels::hasNext() const
{
    return this->loop || *this->it != this->channels[this->currentChannel].end();
}

}}