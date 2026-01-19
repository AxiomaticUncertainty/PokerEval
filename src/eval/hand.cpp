#include "../game/card.h"
#include "hand.h"

#include <optional>
#include <sstream>
#include <utility>

namespace eval {

Evaluation::Evaluation(HandType type, std::array<std::size_t, 5>&& context) : type(type), context(std::move(context)) {}

bool Evaluation::operator<(const Evaluation& other) const {
    if (this->type != other.type)
        return static_cast<int>(this->type) > static_cast<int>(other.type);

    for (std::size_t i = 0; i < 5; ++i) {
        if (this->context[i] > other.context[i])
            return false; // higher card somewhere => better hand
    }

    return true;
}

bool Evaluation::operator>=(const Evaluation& other) const {
    return !(*this < other);
}

bool Evaluation::operator==(const Evaluation& other) const {
    if (this->type != other.type)
        return false;

    for (std::size_t i = 0; i < 5; ++i) {
        if (this->context[i] != other.context[i])
            return false;
    }
    
    return true;
}

std::string handToString(const Evaluation& evaluation) {
    std::stringstream ss;

    switch (evaluation.type) {
    case Evaluation::HandType::StraightFlush: {
        if (evaluation.context[0] == 14) {
            ss << "Royal Flush";
        } else {
            ss << "Straight Flush: " << game::rawToCard(evaluation.context[0]) << " high";
        }
        break;
    }
    case Evaluation::HandType::Quads: {
        ss << "Four of a Kind: " << game::rawToCard(evaluation.context[0]) << "s with "
        << game::rawToCard(evaluation.context[1]) << " kicker";
        break;
    }
    case Evaluation::HandType::FullHouse: {
        ss << "Full House: three " << game::rawToCard(evaluation.context[0]) << "s and a pair of "
        << game::rawToCard(evaluation.context[1]) << "s";
        break;
    }
    case Evaluation::HandType::Flush: {
        ss << "Flush: ";
        for (std::size_t i = 0; i < 5; ++i) {
            ss << game::rawToCard(evaluation.context[i]);
            if (i != 4) {
                ss << ", ";
            }
        }
        break;
    }
    case Evaluation::HandType::Straight: {
        ss << "Straight: " << game::rawToCard(evaluation.context[0]) << " high";
        break;
    }
    case Evaluation::HandType::Trips: {
        ss << "Three of a Kind: " << game::rawToCard(evaluation.context[0]) << "s with "
        << game::rawToCard(evaluation.context[1]) << " and " << game::rawToCard(evaluation.context[2]);
        break;
    }
    case Evaluation::HandType::TwoPair: {
        ss << "Two Pair: " << game::rawToCard(evaluation.context[0]) << "s and "
        << game::rawToCard(evaluation.context[1]) << "s with "
        << game::rawToCard(evaluation.context[2]) << " kicker";
        break;
    }
    case Evaluation::HandType::Pair: {
        ss << "Pair: " << game::rawToCard(evaluation.context[0]) << "s with "
        << game::rawToCard(evaluation.context[1]) << ", "
        << game::rawToCard(evaluation.context[2]) << ", and "
        << game::rawToCard(evaluation.context[3]);
        break;
    }
    case Evaluation::HandType::HighCard: {
        ss << "High Card: ";
        for (std::size_t i = 0; i < 5; ++i) {
            ss << game::rawToCard(evaluation.context[i]);
            if (i != 4) {
                ss << ", ";
            }
        }
        break;
    }
    }

    return ss.str();
}

Hand::Hand() : m_cards(), m_valueSets(), m_flushSets(), m_numCards(0) {}

void Hand::reset() {
    for (auto& suit : m_cards)
        suit.fill(false);
    
    m_valueSets.fill(0);
    m_flushSets.fill(0);
    m_numCards = 0;
}

void Hand::addCard(const game::Card& card) {
    auto suitIndex = static_cast<std::size_t>(card.suit());
    auto valueIndex = card.number() - 2;

    if (m_numCards == 7 || m_cards[suitIndex][valueIndex])
        throw std::runtime_error("Can't add the card");

    ++m_valueSets[valueIndex];
    ++m_flushSets[suitIndex];
    m_cards[suitIndex][valueIndex] = true;

    ++m_numCards;
}

Evaluation Hand::evaluate() const {
    if (m_numCards != 7)
        throw std::runtime_error("Can't evaluate incomplete hand");

    std::optional<game::Suit> flush = this->flush();
    std::optional<std::size_t> straight = this->straight();

    // best hand
    if (flush.has_value() && straight.has_value()) {
        std::optional<std::size_t> straightFlush = this->straightFlush(flush.value());
        if (straightFlush.has_value()) {
            return Evaluation(Evaluation::HandType::StraightFlush, {straightFlush.value(), 0, 0, 0, 0});
        }
    }

    auto sets = this->sets();
    auto& singles = sets[0];
    auto& pairs = sets[1];
    auto& triples = sets[2];
    auto& quads = sets[3];

    // second best hand
    if (!quads.empty()) {
        auto kicker = singles[0] == quads[0] ? singles[1] : singles[0];
        return Evaluation(Evaluation::HandType::Quads, {quads[0], kicker, 0, 0, 0});
    }

    // third best hand
    if (!triples.empty() && pairs.size() >= 2) {
        return Evaluation(Evaluation::HandType::FullHouse, {triples[0], pairs[0] == triples[0] ? pairs[1] : pairs[0], 0, 0, 0});
    }

    // fourth best hand
    if (flush.has_value()) {
        std::array<std::size_t, 5> context;
        std::size_t i = 12;
        int count = 0;
        while (count < 5) {
            if (m_cards[static_cast<std::size_t>(flush.value())][i]) {
                context[count] = i + 2;
                ++count;
            }
            --i;
        }

        return Evaluation(Evaluation::HandType::Flush, std::move(context));
    }

    // fifth best hand
    if (straight.has_value()) {
        return Evaluation(Evaluation::HandType::Straight, {straight.value(), 0, 0, 0, 0});
    }

    // sixth best hand
    if (!triples.empty()) {
        auto highKicker = triples[0] == singles[0] ? singles[1] : singles[0];
        auto lowKicker = triples[0] == singles[0] || triples[0] == singles[1] ? singles[2] : singles[1];
        return Evaluation(Evaluation::HandType::Trips, {triples[0], highKicker, lowKicker, 0, 0});
    }

    // seventh best hand
    if (pairs.size() >= 2) {
        std::size_t kicker;
        if (pairs[0] != singles[0]) {
            kicker = singles[0];
        } else if (pairs[1] != singles[1]) {
            kicker = singles[1];
        } else {
            kicker = singles[2];
        }
        return Evaluation(Evaluation::HandType::TwoPair, {pairs[0], pairs[1], kicker, 0, 0});
    }

    // eigth best hand
    if (!pairs.empty()) {
        std::array<std::size_t, 5> context;
        context[0] = pairs[0];
        std::size_t i = 0, total = 0;
        while (total < 3) {
            if (singles[i] != pairs[0]) {
                context[total + 1] = singles[i];
                ++total;
            }
            ++i;
        }
        
        return Evaluation(Evaluation::HandType::Pair, std::move(context));
    }

    // worst hand
    return Evaluation(Evaluation::HandType::HighCard, {singles[0], singles[1], singles[2], singles[3], singles[4]}); // works since there are no pairs
}

std::optional<game::Suit> Hand::flush() const {
    for (std::size_t s = 0; s < 4; ++s) {
        if (m_flushSets[s] >= 5) {
            return static_cast<game::Suit>(s);
        }
    }

    return std::optional<game::Suit>();
}

std::optional<std::size_t> Hand::straight() const {
    std::optional<std::size_t> high;
    for (std::size_t start = 0, i = 0; i < 13; ++i) {
        if (m_valueSets[i] == 0) {
            start = i + 1;
        } else if (i - start + 1 >= 5 || (i - start + 1 >= 4 && start == 0 && m_valueSets[12] > 0)) {
            high = i + 2;
        }
    }

    return high;
}

std::optional<std::size_t> Hand::straightFlush(game::Suit suit) const {
    std::optional<std::size_t> high;
    for (std::size_t start = 0, i = 0; i < 13; ++i) {
        if (m_cards[static_cast<std::size_t>(suit)][i] == 0) {
            start = i + 1;
            } else if (i - start + 1 >= 5 || (i - start + 1 >= 4 && start == 0 && m_cards[static_cast<std::size_t>(suit)][12] > 0)) {
            high = i + 2;
        }
    }

    return high;
}

std::array<Hand::CardSet, 4> Hand::sets() const {
    std::array<Hand::CardSet, 4> setsInHand;
    for (int i = 12; i >= 0; --i) {
        for (std::size_t set = 0; set < m_valueSets[static_cast<std::size_t>(i)]; ++set) {
            setsInHand[set].push_back(i + 2);
        }
    }
    return setsInHand;
}

}