#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"

// Константи
const int MAX_CARDS_IN_HAND = 3;

// Структура за играч
struct Player {
    int id;                     // Уникален ID на играча
    int balance;                // Баланс на играча
    Card hand[MAX_CARDS_IN_HAND]; // Ръката на играча
    bool isActive;              // Активност на играча
};

// Функции за играч
void initializePlayer(Player& player, int id, int initialBalance);
void printPlayerHand(const Player& player);

#endif
