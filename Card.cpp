#include "Card.h"
#include <iostream>

// Печат на карта
void printCard(const Card& card) {
    std::string suits[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
    std::string ranks[] = { "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };

    std::cout << ranks[card.rank - 7] << " of " << suits[card.suit] << std::endl;
}
