#include "Flipbook.h"

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

Flipbook::flipbook_iterator::flipbook_iterator(const Flipbook& fb, size_t idx)
    : fb(fb), idx(idx)
{
    if (fb.size() > idx)
        card = fb.cards[idx];
}

const wkt::math::Rect& Flipbook::flipbook_iterator::operator*() const
{
    return this->card.rect;
}

Flipbook::flipbook_iterator& Flipbook::flipbook_iterator::operator++()
{
    if (this->idx >= this->fb.size())
        return *this;
    
    if (this->card.times == 1 && this->idx + 1 < this->fb.size())
    {
        this->idx++;
        this->card = this->fb.cards[this->idx];
    }
    else if (this->card.times > 0)
        this->card.times--;
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

wkt::math::Rect FlipbookChannels::next()
{
    if (this->loop && *this->it == this->channels[this->currentChannel].end())
    {
        setChannel(this->currentChannel, this->loop);
    }

    wkt::math::Rect r = **this->it;
    ++*this->it;
    return r;
}

bool FlipbookChannels::hasNext() const
{
    return this->loop || *this->it != this->channels[this->currentChannel].end();
}

}}