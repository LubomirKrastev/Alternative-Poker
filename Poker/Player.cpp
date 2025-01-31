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
* <file with player logic>
*
*/

#include "Player.h"
#include <iostream>

// Инициализиране на играч динамично
void initializePlayer(Player& player, int id, int initialBalance) {
    player.id = id;
    player.balance = initialBalance;
    player.isActive = true;
    player.hand = new Card[MAX_CARDS_IN_HAND];
}

// Принтиране на ръка
void printPlayerHand(const Player* player) {
    for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
        printCard(&player->hand[i]);
    }
    std::cout << std::endl;
}
