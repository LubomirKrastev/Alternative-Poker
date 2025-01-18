#ifndef CARD_H
#define CARD_H

// Константи за боите и картите
enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { SEVEN , EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

// Структура на картата
struct Card {
    Suit suit;
    Rank rank;
};

// Принтиране на карта
void printCard(const Card* card);

#endif
