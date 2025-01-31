/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
*
* @author Lubomir Krastev
* @idnumber 1MI0600441 * @compiler VC
*
* <file with card logic>
*
*/

#include "Card.h"
#include <iostream>

// Създаване на карта динамичо
Card* createCard(Suit suit, Rank rank) {
    Card* card = new Card;
    card->suit = suit;
    card->rank = rank;
    return card;
}

// Принтиране на карта
void printCard(const Card* card) {
    if (!card) {
        return;
    }

    const char* suits[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
    const char* ranks[] = { "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };

    if (card->rank >= SEVEN && card->rank <= ACE) {
        std::cout << ranks[card->rank - SEVEN] << " of " << suits[card->suit] << std::endl;
    }
    else {
        return;
    }
}

// Освобождаване на карта динамично
void freeCard(Card*& card) {
    delete card;
    card = nullptr; 
}
