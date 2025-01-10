#ifndef GAME_H
#define GAME_H

#include "Player.h"

// Константи за играта
const int MAX_PLAYERS = 9;
const int DECK_SIZE = 28;
const int CHIP_VALUE = 10;
const int INITIAL_BALANCE = 100 * CHIP_VALUE;

// Функции за играта
void initializeDeck(Card deck[], int size);
void shuffleDeck(Card deck[], int size);
void dealCards(Player players[], int numPlayers, Card deck[], int& deckIndex);
void playRound(Player players[], int numPlayers, Card deck[], int& deckIndex);
int calculateHandValue(const Player& player);
void startGame();

#endif
