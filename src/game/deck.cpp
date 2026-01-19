#include "deck.h"

#include <algorithm>
#include <random>
#include <utility>

namespace game {

Deck::Deck() : m_cards() {
    for (std::size_t c = 0; c < 52; ++c) {
        m_cards.emplace_back(static_cast<Suit>(c / 13), (c % 13) + 2);
    }
}

void Deck::shuffle() {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::ranges::shuffle(m_cards, rng);
}

Card Deck::takeTop() {
    if (m_cards.empty())
        throw std::runtime_error("Can't draw from an empty deck");

    auto card = std::move(m_cards.back());
    m_cards.pop_back();
    return card;
}

std::span<Card> Deck::viewTop(std::size_t n) {
    if (m_cards.size() < n)
        throw std::runtime_error("Not enough cards to view");

    std::size_t lastCardIndex = m_cards.size() - 1;
    std::size_t nthBack = lastCardIndex - n + 1;

    return std::span<Card>(m_cards.data() + nthBack, n);
}

}