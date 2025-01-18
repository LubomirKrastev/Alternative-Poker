#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"

// Константа
const int MAX_CARDS_IN_HAND = 3;

// Сктруктура на играча
struct Player {
    int id;                     // ID на играча
    int balance;                // Баланс на играча
    Card* hand;                 // Динамично подаване на ръката
    bool isActive;              // Активния статус ма играча
};

// Функции за играча
void initializePlayer(Player& player, int id, int initialBalance);
void printPlayerHand(const Player* player);

#endif
