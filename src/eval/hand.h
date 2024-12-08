#ifndef EVAL_HAND_H_
#define EVAL_HAND_H_

#include "../game/card.h"

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace eval {

struct Evaluation {
    enum class HandType {
        StraightFlush = 2,
        Quads = 3,
        FullHouse = 4,
        Flush = 5,
        Straight = 6,
        Trips = 7,
        TwoPair = 8,
        Pair = 9,
        HighCard = 10
    };

    Evaluation(HandType type, std::array<std::size_t, 5>&& context);

    bool operator<(const Evaluation& other) const;
    bool operator>=(const Evaluation& other) const;
    bool operator==(const Evaluation& other) const;

    HandType type;
    std::array<std::size_t, 5> context;
};

std::string handToString(const Evaluation& evaluation);

class Hand {
public:
    Hand();

    void addCard(const game::Card& card);
    Evaluation evaluate() const;

    // public for testing
    std::optional<game::Suit> flush() const;
    std::optional<std::size_t> straight() const;
    std::optional<std::size_t> straightFlush(game::Suit suit) const;
    std::array<std::vector<std::size_t>, 4> sets() const;

private:
    /*
        m_cards:
        - outer array indexes into a given suit
        - inner array indexes into a given card
          - Two ==> index 0
          - Ace ==> Index 12
    */
    std::array<std::array<bool, 13>, 4> m_cards;
    std::array<std::size_t, 13> m_valueSets; // number of times a given face value appears (m_sets[2] == 3 ==> the hand contains three 4s)
    std::array<std::size_t, 4> m_flushSets;
    std::size_t m_numCards;
};

}

#endif