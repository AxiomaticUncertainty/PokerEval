#include "card.h"

#include <exception>
#include <iostream>
#include <sstream>

namespace game {

Card::Card(Suit suit, std::size_t number) : m_suit(suit), m_number(number) {
    if (number < 2 || number > 14) {
        std::cout << "Invalid card number: " << number << "\n" << std::flush;
        throw std::runtime_error("Invalid card number");
    }
}

Suit Card::suit() const {
    return m_suit;
}

std::size_t Card::number() const {
    return m_number;
}

std::string rawToCard(std::size_t faceValue) {
    std::stringstream ss;
    if (faceValue <= 10) {
        ss << faceValue;
    } else {
        switch (faceValue) {
        case 11: {
            ss << "Jack";
            break;
        }
        case 12: {
            ss << "Queen";
            break;
        }
        case 13: {
            ss << "King";
            break;
        }
        case 14: {
            ss << "Ace";
            break;
        }
        }
    }

    return ss.str();
}

std::ostream& operator<<(std::ostream& os, Suit suit) {
    switch (suit) {
    case Suit::CLUBS: {
        os << "Clubs";
        break;
    }
    case Suit::DIAMONDS: {
        os << "Diamonds";
        break;
    }
    case Suit::HEARTS: {
        os << "Hearts";
        break;
    }
    case Suit::SPADES: {
        os << "Spades";
        break;
    }
    default:
        throw std::runtime_error("Invalid suit");
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << rawToCard(card.number()) << " of " << card.suit();

    return os;
}

bool operator==(const Card& lhs, const Card& rhs) {
    return lhs.suit() == rhs.suit() && lhs.number() == rhs.number();
}

}