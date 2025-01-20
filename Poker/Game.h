#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Card.h"

// Константи за играта
const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 9;
const int DECK_SIZE = 32;
const int CHIP_VALUE = 10;
const int INITIAL_BALANCE = 100 * CHIP_VALUE;

// Функции за играта
void initializeDeck(Card* deck, int size);
void shuffleDeck(Card* deck, int size);
void dealCards(Player* players, int numPlayers, Card* deck, int& deckIndex);
int calculateCardValue(Rank rank);
int calculateHandValue(const Player& player);
bool handleTie(Player* players, int numPlayers, int& pot, int maxHandValue, Card* deck);
int initializeNewGame(Player*& players);
void playRound(Player* players, int numPlayers, Card* deck);
void saveGameState(Player* players, int numPlayers);
void fileManage(int& numPlayers, Player*& players);
void startGame();

#endif
