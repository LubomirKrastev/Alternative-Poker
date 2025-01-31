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
* <file with main logic>
*
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"

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
