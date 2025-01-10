#include "Player.h"
#include <iostream>

// Инициализация на играч
void initializePlayer(Player& player, int id, int initialBalance) {
    player.id = id;
    player.balance = initialBalance;
    player.isActive = true;
}

// Печат на ръката на играча
void printPlayerHand(const Player& player) {
    for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
        printCard(player.hand[i]);
    }
    std::cout << std::endl;
}
