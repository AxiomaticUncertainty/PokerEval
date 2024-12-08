#include "../game/deck.h"
#include "hand.h"
#include "monte.h"

#include <ranges>
#include <unordered_set>
#include <vector>

namespace eval {

namespace {

auto pickWinner = [](const auto& a, const auto& b) {
    auto winner = a < b ? b : a;
    auto loser = a < b ? a : b;

    std::cout << "Winner: " << eval::handToString(winner) << "; loser: " << eval::handToString(loser) << "\n";
};

}

Simulator::Results Simulator::operator()(const std::pair<game::Card, game::Card>& hand, std::size_t tableSize, std::size_t runs) const {
    std::unordered_set<game::Card, game::Card::Hash> omittedCards = {
        hand.first,
        hand.second
    };

    game::Deck deck(omittedCards);

    Results aggregateResults;
    for (auto _ : std::views::iota(0ul, runs)) {
        deck.shuffle();

        std::size_t drawSize = 2 * (tableSize - 1) + 5;
        auto cards = deck.viewTop(drawSize);

        std::vector<eval::Hand> hands(tableSize);
        
        for (std::size_t h = 0; h < tableSize; ++h) {
            if (h == 0) {
                hands[h].addCard(hand.first);
                hands[h].addCard(hand.second);
            } else {
                hands[h].addCard(cards[(h - 1) * 2]);
                hands[h].addCard(cards[(h - 1) * 2 + 1]);
            }

            for (std::size_t c = drawSize - 5; c < drawSize; ++c) {
                hands[h].addCard(cards[c]);
            }
        }
        
        bool win = true;
        auto simHandEval = hands[0].evaluate();
        for (std::size_t h = 1; h < hands.size(); ++h) {
            auto eval = hands[h].evaluate();

            if (simHandEval < eval) {
                win = false;
                break;
            }
        }

        if (win)
            ++aggregateResults.first;

        ++aggregateResults.second;
    }

    return aggregateResults;
}

}