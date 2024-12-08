#include "eval/hand.h"
#include "game/card.h"
#include "game/deck.h"

#include <iostream>
#include <unordered_set>

void testCards() {
    game::Card diamondQueen(game::Suit::DIAMONDS, 12);
    game::Card twoSpades(game::Suit::SPADES, 2);
    game::Card aceHearts(game::Suit::HEARTS, 14);

    std::cout << diamondQueen << "\n";
    std::cout << twoSpades << "\n";
    std::cout << aceHearts << "\n";
}

void testDeck() {
    game::Deck deck;
    deck.shuffle();
    for (std::size_t _ = 0; _ < 10; ++_) {
        game::Card c = deck.takeTop();
        std::cout << c << "\n";
    }
}

void testDeckWithOmission() {
    std::unordered_set<game::Card, game::Card::Hash> omittedCards = {
        game::Card(game::Suit::CLUBS, 14),
        game::Card(game::Suit::SPADES, 14)
    };

    game::Deck deck(omittedCards);
    deck.shuffle();
    for (std::size_t _ = 0; _ < 50; ++_) {
        game::Card c = deck.takeTop();
        std::cout << c << "\n";
    }
}

void testFlushEvaluation() {
    game::Card two(game::Suit::CLUBS, 2);
    game::Card five(game::Suit::CLUBS, 5);
    game::Card eight(game::Suit::CLUBS, 8);
    game::Card eightOffSuit(game::Suit::DIAMONDS, 8);
    game::Card eleven(game::Suit::CLUBS, 11);
    game::Card three(game::Suit::CLUBS, 3);
    game::Card aceOffSuit(game::Suit::SPADES, 14);
    game::Card kingOffSuit(game::Suit::SPADES, 13);

    eval::Hand flushHand;
    flushHand.addCard(two);
    flushHand.addCard(five);
    flushHand.addCard(eight);
    flushHand.addCard(eightOffSuit);
    flushHand.addCard(eleven);
    flushHand.addCard(three);
    flushHand.addCard(aceOffSuit);

    eval::Hand nonFlushHand;
    nonFlushHand.addCard(two);
    nonFlushHand.addCard(five);
    nonFlushHand.addCard(eight);
    nonFlushHand.addCard(eightOffSuit);
    nonFlushHand.addCard(eleven);
    nonFlushHand.addCard(aceOffSuit);
    nonFlushHand.addCard(kingOffSuit);

    auto evaluateFlush = [](auto flush) {
        if (flush.has_value()) {
            std::cout << "Flush: " << flush.value() << "\n";
        } else {
            std::cout << "No flush\n";
        }
    };

    evaluateFlush(flushHand.flush());
    evaluateFlush(nonFlushHand.flush());
}

void testStraightEvaluation() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);

    eval::Hand lowStraightHand;
    lowStraightHand.addCard(aceClubs);
    lowStraightHand.addCard(twoDiamonds);
    lowStraightHand.addCard(threeClubs);
    lowStraightHand.addCard(fourSpades);
    lowStraightHand.addCard(fiveHearts);
    lowStraightHand.addCard(kingClubs);
    lowStraightHand.addCard(queenClubs);

    eval::Hand highStraightHand;
    highStraightHand.addCard(jackClubs);
    highStraightHand.addCard(fourSpades);
    highStraightHand.addCard(fiveHearts);
    highStraightHand.addCard(kingClubs);
    highStraightHand.addCard(queenClubs);
    highStraightHand.addCard(tenClubs);
    highStraightHand.addCard(aceClubs);

    eval::Hand noStraightHand;
    noStraightHand.addCard(jackClubs);
    noStraightHand.addCard(fourSpades);
    noStraightHand.addCard(fiveHearts);
    noStraightHand.addCard(kingClubs);
    noStraightHand.addCard(threeClubs);
    noStraightHand.addCard(tenClubs);
    noStraightHand.addCard(aceClubs);

    auto evaluate = [](auto straight) {
        if (straight.has_value()) {
            std::cout << "Straight: " << game::rawToCard(straight.value()) << " high\n";
        } else {
            std::cout << "No straight\n";
        }
    };

    evaluate(lowStraightHand.straight());
    evaluate(highStraightHand.straight());
    evaluate(noStraightHand.straight());
}

void testStraightFlushEvaluation() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card twoClubs(game::Suit::CLUBS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card sixClubs(game::Suit::CLUBS, 6);

    eval::Hand straightFlush;
    straightFlush.addCard(aceClubs);
    straightFlush.addCard(tenClubs);
    straightFlush.addCard(jackClubs);
    straightFlush.addCard(queenClubs);
    straightFlush.addCard(kingClubs);
    straightFlush.addCard(fourSpades);
    straightFlush.addCard(fiveHearts);

    eval::Hand straightDisjointFlush;
    straightDisjointFlush.addCard(aceClubs);
    straightDisjointFlush.addCard(twoClubs);
    straightDisjointFlush.addCard(threeClubs);
    straightDisjointFlush.addCard(fourSpades);
    straightDisjointFlush.addCard(fiveHearts);
    straightDisjointFlush.addCard(sixClubs);
    straightDisjointFlush.addCard(tenClubs);

    auto evaluate = [](auto straight) {
        if (straight.has_value()) {
            std::cout << "Straight flush: " << game::rawToCard(straight.value()) << " high\n";
        } else {
            std::cout << "No straight flush\n";
        }
    };

    evaluate(straightFlush.straightFlush(game::Suit::CLUBS));
    evaluate(straightDisjointFlush.straightFlush(game::Suit::CLUBS));
}

void testStraightFlushHand() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card twoClubs(game::Suit::CLUBS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card sixClubs(game::Suit::CLUBS, 6);

    eval::Hand straightFlush;
    straightFlush.addCard(aceClubs);
    straightFlush.addCard(tenClubs);
    straightFlush.addCard(jackClubs);
    straightFlush.addCard(queenClubs);
    straightFlush.addCard(kingClubs);
    straightFlush.addCard(fourSpades);
    straightFlush.addCard(fiveHearts);

    std::cout << eval::handToString(straightFlush.evaluate()) << "\n";
}

void testQuadsHand() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card aceSpades(game::Suit::SPADES, 14);
    game::Card aceHearts(game::Suit::HEARTS, 14);
    game::Card aceDiamonds(game::Suit::DIAMONDS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card twoClubs(game::Suit::CLUBS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);

    eval::Hand quads;
    quads.addCard(aceClubs);
    quads.addCard(aceDiamonds);
    quads.addCard(aceSpades);
    quads.addCard(aceHearts);
    quads.addCard(kingClubs);
    quads.addCard(fourSpades);
    quads.addCard(fiveHearts);

    auto eval = quads.evaluate();
    std::cout << eval::handToString(eval)<< "\n";
}

void testFullHouseHands() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card aceSpades(game::Suit::SPADES, 14);
    game::Card aceHearts(game::Suit::HEARTS, 14);
    game::Card aceDiamonds(game::Suit::DIAMONDS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card twoClubs(game::Suit::CLUBS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card fiveSpades(game::Suit::SPADES, 5);
    game::Card fiveDiamonds(game::Suit::DIAMONDS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);

    eval::Hand tripleHigh;
    tripleHigh.addCard(aceClubs);
    tripleHigh.addCard(aceHearts);
    tripleHigh.addCard(aceDiamonds);
    tripleHigh.addCard(twoClubs);
    tripleHigh.addCard(twoDiamonds);
    tripleHigh.addCard(queenClubs);
    tripleHigh.addCard(fiveSpades);

    eval::Hand tripleHighTwoPair;
    tripleHighTwoPair.addCard(aceClubs);
    tripleHighTwoPair.addCard(aceHearts);
    tripleHighTwoPair.addCard(aceDiamonds);
    tripleHighTwoPair.addCard(fiveHearts);
    tripleHighTwoPair.addCard(fiveSpades);
    tripleHighTwoPair.addCard(twoDiamonds);
    tripleHighTwoPair.addCard(twoClubs);

    eval::Hand twoTriples;
    twoTriples.addCard(aceClubs);
    twoTriples.addCard(aceHearts);
    twoTriples.addCard(aceDiamonds);
    twoTriples.addCard(fiveDiamonds);
    twoTriples.addCard(fiveHearts);
    twoTriples.addCard(fiveSpades);
    twoTriples.addCard(kingClubs);

    std::cout << eval::handToString(tripleHigh.evaluate()) << "\n";
    std::cout << eval::handToString(tripleHighTwoPair.evaluate()) << "\n";
    std::cout << eval::handToString(twoTriples.evaluate()) << "\n";
}

void testFlushHands() {
    game::Card two(game::Suit::CLUBS, 2);
    game::Card five(game::Suit::CLUBS, 5);
    game::Card eight(game::Suit::CLUBS, 8);
    game::Card eightOffSuit(game::Suit::DIAMONDS, 8);
    game::Card jack(game::Suit::CLUBS, 11);
    game::Card queen(game::Suit::CLUBS, 12);
    game::Card three(game::Suit::CLUBS, 3);
    game::Card aceOffSuit(game::Suit::SPADES, 14);
    game::Card kingOffSuit(game::Suit::SPADES, 13);

    eval::Hand flushHand;
    flushHand.addCard(two);
    flushHand.addCard(five);
    flushHand.addCard(eight);
    flushHand.addCard(eightOffSuit);
    flushHand.addCard(jack);
    flushHand.addCard(three);
    flushHand.addCard(aceOffSuit);

    eval::Hand flushHandExtraCard;
    flushHandExtraCard.addCard(two);
    flushHandExtraCard.addCard(five);
    flushHandExtraCard.addCard(eight);
    flushHandExtraCard.addCard(eightOffSuit);
    flushHandExtraCard.addCard(jack);
    flushHandExtraCard.addCard(three);
    flushHandExtraCard.addCard(queen);

    std::cout << eval::handToString(flushHand.evaluate()) << "\n";
    std::cout << eval::handToString(flushHandExtraCard.evaluate()) << "\n";
}

void testStraightHands() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card nineHearts(game::Suit::HEARTS, 9);

    eval::Hand lowStraightHand;
    lowStraightHand.addCard(aceClubs);
    lowStraightHand.addCard(twoDiamonds);
    lowStraightHand.addCard(threeClubs);
    lowStraightHand.addCard(fourSpades);
    lowStraightHand.addCard(fiveHearts);
    lowStraightHand.addCard(kingClubs);
    lowStraightHand.addCard(queenClubs);

    eval::Hand highStraightHand;
    highStraightHand.addCard(jackClubs);
    highStraightHand.addCard(fourSpades);
    highStraightHand.addCard(fiveHearts);
    highStraightHand.addCard(kingClubs);
    highStraightHand.addCard(queenClubs);
    highStraightHand.addCard(tenClubs);
    highStraightHand.addCard(nineHearts);

    std::cout << eval::handToString(lowStraightHand.evaluate()) << "\n";
    std::cout << eval::handToString(highStraightHand.evaluate()) << "\n";
}

void testTripleHands() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card kingSpades(game::Suit::SPADES, 13);
    game::Card kingDiamonds(game::Suit::DIAMONDS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card nineHearts(game::Suit::HEARTS, 9);

    eval::Hand highTriple;
    highTriple.addCard(kingClubs);
    highTriple.addCard(kingSpades);
    highTriple.addCard(kingDiamonds);
    highTriple.addCard(jackClubs);
    highTriple.addCard(fiveHearts);
    highTriple.addCard(nineHearts);
    highTriple.addCard(fourSpades);

    eval::Hand splitTriple;
    splitTriple.addCard(kingClubs);
    splitTriple.addCard(kingSpades);
    splitTriple.addCard(kingDiamonds);
    splitTriple.addCard(aceClubs);
    splitTriple.addCard(fiveHearts);
    splitTriple.addCard(nineHearts);
    splitTriple.addCard(fourSpades);

    std::cout << eval::handToString(highTriple.evaluate()) << "\n";
    std::cout << eval::handToString(splitTriple.evaluate()) << "\n";
}

void testTwoPairHands() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card aceHearts(game::Suit::HEARTS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card kingSpades(game::Suit::SPADES, 13);
    game::Card kingDiamonds(game::Suit::DIAMONDS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card nineHearts(game::Suit::HEARTS, 9);
    game::Card nineDiamonds(game::Suit::DIAMONDS, 9);

    eval::Hand twoOverKicker;
    twoOverKicker.addCard(aceClubs);
    twoOverKicker.addCard(aceHearts);
    twoOverKicker.addCard(twoDiamonds);
    twoOverKicker.addCard(jackClubs);
    twoOverKicker.addCard(fourSpades);
    twoOverKicker.addCard(kingClubs);
    twoOverKicker.addCard(kingSpades);

    eval::Hand overUnderKicker;
    overUnderKicker.addCard(twoDiamonds);
    overUnderKicker.addCard(jackClubs);
    overUnderKicker.addCard(fourSpades);
    overUnderKicker.addCard(kingClubs);
    overUnderKicker.addCard(kingSpades);
    overUnderKicker.addCard(nineDiamonds);
    overUnderKicker.addCard(nineHearts);

    eval::Hand twoUnderKicker;
    twoUnderKicker.addCard(aceClubs);
    twoUnderKicker.addCard(jackClubs);
    twoUnderKicker.addCard(fourSpades);
    twoUnderKicker.addCard(kingClubs);
    twoUnderKicker.addCard(kingSpades);
    twoUnderKicker.addCard(nineDiamonds);
    twoUnderKicker.addCard(nineHearts);

    std::cout << eval::handToString(twoOverKicker.evaluate()) << "\n";
    std::cout << eval::handToString(overUnderKicker.evaluate()) << "\n";
    std::cout << eval::handToString(twoUnderKicker.evaluate()) << "\n";
}

void testPairHand() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card aceHearts(game::Suit::HEARTS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card kingSpades(game::Suit::SPADES, 13);
    game::Card kingDiamonds(game::Suit::DIAMONDS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card nineHearts(game::Suit::HEARTS, 9);
    game::Card nineDiamonds(game::Suit::DIAMONDS, 9);

    eval::Hand simplePair;
    simplePair.addCard(kingSpades);
    simplePair.addCard(kingDiamonds);
    simplePair.addCard(aceClubs);
    simplePair.addCard(nineDiamonds);
    simplePair.addCard(queenClubs);
    simplePair.addCard(twoDiamonds);
    simplePair.addCard(fourSpades);

    std::cout << eval::handToString(simplePair.evaluate()) << "\n";
}

void testHighCardHand() {
    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card aceHearts(game::Suit::HEARTS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card kingSpades(game::Suit::SPADES, 13);
    game::Card kingDiamonds(game::Suit::DIAMONDS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card nineHearts(game::Suit::HEARTS, 9);
    game::Card nineDiamonds(game::Suit::DIAMONDS, 9);

    eval::Hand highCard;
    highCard.addCard(aceClubs);
    highCard.addCard(kingSpades);
    highCard.addCard(queenClubs);
    highCard.addCard(jackClubs);
    highCard.addCard(nineDiamonds);
    highCard.addCard(fiveHearts);
    highCard.addCard(twoDiamonds);

    std::cout << eval::handToString(highCard.evaluate()) << "\n";
}

void testHandComparison() {
    game::Card two(game::Suit::CLUBS, 2);
    game::Card five(game::Suit::CLUBS, 5);
    game::Card eight(game::Suit::CLUBS, 8);
    game::Card eightOffSuit(game::Suit::DIAMONDS, 8);
    game::Card jack(game::Suit::CLUBS, 11);
    game::Card queen(game::Suit::CLUBS, 12);
    game::Card three(game::Suit::CLUBS, 3);
    game::Card aceOffSuit(game::Suit::SPADES, 14);
    game::Card kingOffSuit(game::Suit::SPADES, 13);

    eval::Hand flushHand;
    flushHand.addCard(two);
    flushHand.addCard(five);
    flushHand.addCard(eight);
    flushHand.addCard(eightOffSuit);
    flushHand.addCard(jack);
    flushHand.addCard(three);
    flushHand.addCard(aceOffSuit);

    game::Card aceClubs(game::Suit::CLUBS, 14);
    game::Card aceSpades(game::Suit::SPADES, 14);
    game::Card aceHearts(game::Suit::HEARTS, 14);
    game::Card aceDiamonds(game::Suit::DIAMONDS, 14);
    game::Card twoDiamonds(game::Suit::DIAMONDS, 2);
    game::Card twoClubs(game::Suit::CLUBS, 2);
    game::Card threeClubs(game::Suit::CLUBS, 3);
    game::Card fourSpades(game::Suit::SPADES, 4);
    game::Card fiveHearts(game::Suit::HEARTS, 5);
    game::Card fiveSpades(game::Suit::SPADES, 5);
    game::Card fiveDiamonds(game::Suit::DIAMONDS, 5);
    game::Card kingClubs(game::Suit::CLUBS, 13);
    game::Card queenClubs(game::Suit::CLUBS, 12);

    eval::Hand tripleHigh;
    tripleHigh.addCard(aceClubs);
    tripleHigh.addCard(aceHearts);
    tripleHigh.addCard(aceDiamonds);
    tripleHigh.addCard(twoClubs);
    tripleHigh.addCard(twoDiamonds);
    tripleHigh.addCard(queenClubs);
    tripleHigh.addCard(fiveSpades);

    game::Card kingSpades(game::Suit::SPADES, 13);
    game::Card kingDiamonds(game::Suit::DIAMONDS, 13);
    game::Card jackClubs(game::Suit::CLUBS, 11);
    game::Card tenClubs(game::Suit::CLUBS, 10);
    game::Card nineHearts(game::Suit::HEARTS, 9);

    eval::Hand highTriple;
    highTriple.addCard(kingClubs);
    highTriple.addCard(kingSpades);
    highTriple.addCard(kingDiamonds);
    highTriple.addCard(jackClubs);
    highTriple.addCard(fiveHearts);
    highTriple.addCard(nineHearts);
    highTriple.addCard(fourSpades);

    eval::Hand splitTriple;
    splitTriple.addCard(kingClubs);
    splitTriple.addCard(kingSpades);
    splitTriple.addCard(kingDiamonds);
    splitTriple.addCard(aceClubs);
    splitTriple.addCard(fiveHearts);
    splitTriple.addCard(nineHearts);
    splitTriple.addCard(fourSpades);

    auto flush = flushHand.evaluate();
    auto fullHouse = tripleHigh.evaluate();
    auto threeKings = highTriple.evaluate();
    auto threeKingsAceHigh = splitTriple.evaluate();

    auto pickWinner = [](const auto& a, const auto& b) {
        auto winner = a < b ? b : a;
        auto loser = a < b ? a : b;

        std::cout << "Winner: " << eval::handToString(winner) << "; loser: " << eval::handToString(loser) << "\n";
    };
    
    pickWinner(flush, fullHouse);
    pickWinner(threeKings, flush);
    pickWinner(threeKings, fullHouse);
    pickWinner(threeKingsAceHigh, threeKings);
}

int main() {
    testCards();
    testDeck();
    testDeckWithOmission();
    testFlushEvaluation();
    testStraightEvaluation();
    testStraightFlushEvaluation();
    testStraightFlushHand();
    testQuadsHand();
    testFullHouseHands();
    testFlushHands();
    testStraightHands();
    testTripleHands();
    testTwoPairHands();
    testPairHand();
    testHighCardHand();
    testHandComparison();

    return 0;
}