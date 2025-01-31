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
void dealCardsToActivePlayers(Player* players, int numPlayers, Card* deck, int& deckIndex);
bool handleTie(Player* players, int numPlayers, int& pot, int maxHandValue, Card* deck);
void removePlayersWithZeroBalance(Player*& players, int& numPlayers);
int initializeNewGame(Player*& players);
void playRound(Player* players, int numPlayers, Card* deck, int& pot);
void fileManage(int& numPlayers, Player*& players);
void saveGameState(Player* players, int numPlayers);
void startGame();

#endif
