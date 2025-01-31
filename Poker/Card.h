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
