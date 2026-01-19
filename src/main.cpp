#include "game/card.h"
#include "eval/monte.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

void prettyResults(const auto& result) {
    std::cout << "Won " << result.first << " of " << result.second << " total hands\n";
    std::cout << "Winrate = " << (result.first * 1.0 / result.second) * 100 << "%\n";
}

void h2h() {
    eval::Simulator simulate;
    auto result = simulate(
        {game::Card(game::Suit::CLUBS, 14), game::Card(game::Suit::SPADES, 14)},
        2,
        10'000'000
    );

    prettyResults(result);
}

std::mutex mutex;

void h2ht() {
    std::vector<std::thread> threads;
    eval::Simulator::Results shared_results;
    for (std::size_t t = 0; t < 1; ++t) {
        threads.emplace_back([&shared_results] {
            eval::Simulator simulate;
            auto result = simulate(
                {game::Card(game::Suit::CLUBS, 14), game::Card(game::Suit::SPADES, 14)},
                2,
                1'000'000
            );

            std::lock_guard lock(mutex);
            shared_results.first += result.first;
            shared_results.second += result.second;
        });
    }

    for (auto& thread : threads)
        thread.join();

    prettyResults(shared_results);
}

void allCombinations() {
    eval::Simulator simulate;

    for (bool suited : {true, false}) {
        if (suited)
            std::cout << "------- Suited results: ------\n\n";
        else
            std::cout << "----- Offsuited results: -----\n\n";

        for (std::size_t i = 2; i <= 14; ++i) {
            for (std::size_t j = suited ? i + 1 : i; j <= 14; ++j) {
                auto results = simulate(
                    {
                        game::Card(game::Suit::DIAMONDS, i),
                        game::Card(suited ? game::Suit::DIAMONDS : game::Suit::HEARTS, j)
                    },
                    6,
                    100000
                );

                std::cout << game::rawToCard(i) << " & " << game::rawToCard(j) << ":\n";
                prettyResults(results);
                std::cout << "\n";
            }
        }
    }
}

int main() {
    h2ht();
    // allCombinations();

    return 0;
}