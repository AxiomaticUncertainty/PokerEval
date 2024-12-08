#ifndef GAME_DECK_H_
#define GAME_DECK_H_

#include "card.h"

#include <span>
#include <unordered_set>
#include <vector>

namespace game {

class Deck {
public:
    Deck();
    Deck(const auto& omitted);

    void shuffle();

    Card takeTop();
    
    std::span<Card> viewTop(std::size_t n);

private:
    std::vector<Card> m_cards;
};

Deck::Deck(const auto& omitted) : m_cards() {
    for (std::size_t c = 0; c < 52; ++c) {
        Card card(static_cast<Suit>(c / 13), (c % 13) + 2);
        
        if (omitted.contains(card))
            continue;

        m_cards.push_back(std::move(card));
    }
}

}

#endif