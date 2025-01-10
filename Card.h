#ifndef CARD_H
#define CARD_H

// Константи за боите и ранговете на картите
enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { SEVEN = 7, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

// Структура за карта
struct Card {
    Suit suit;
    Rank rank;
};

// Функции за работа с карти
void printCard(const Card& card);

#endif
