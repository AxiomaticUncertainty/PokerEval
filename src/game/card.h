#ifndef GAME_CARD_H_
#define GAME_CARD_H_

#include <cstdint>
#include <iostream>
#include <string>

namespace game {

enum class Suit {
    DIAMONDS = 0,
    CLUBS = 1,
    HEARTS = 2,
    SPADES = 3
};

class Card {
public:
    Card(Suit suit, std::size_t number);

    Suit suit() const;
    std::size_t number() const;

    struct Hash {
        std::size_t operator()(const Card& card) const {
            return static_cast<std::size_t>(card.suit()) + (card.number() << 2);
        }
    };

private:
    Suit m_suit;
    std::size_t m_number;
};

std::ostream& operator<<(std::ostream& os, Suit suit);
std::ostream& operator<<(std::ostream& os, const Card& card);

bool operator==(const Card& lhs, const Card& rhs);

std::string rawToCard(std::size_t faceValue);

}

#endif