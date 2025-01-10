#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

// Инициализация на тестето
void initializeDeck(Card deck[], int size) {
    int index = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = SEVEN; rank <= ACE; rank++) {
            deck[index].suit = static_cast<Suit>(suit);
            deck[index].rank = static_cast<Rank>(rank);
            index++;
        }
    }
}

// Разбъркване на тестето
void shuffleDeck(Card deck[], int size) {
    srand(time(0));
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Раздаване на карти
void dealCards(Player players[], int numPlayers, Card deck[], int& deckIndex) {
    if (deckIndex + (numPlayers * MAX_CARDS_IN_HAND) > DECK_SIZE) {
        std::cout << "Deck is out of cards! Reshuffling...\n";
        shuffleDeck(deck, DECK_SIZE);
        deckIndex = 0;
    }

    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < MAX_CARDS_IN_HAND; j++) {
            if (deckIndex >= DECK_SIZE) {
                std::cerr << "Error: Deck index out of bounds!\n";
                return;
            }
            players[i].hand[j] = deck[deckIndex++];
        }
    }
}


// Изчисляване на стойността на ръката
int calculateHandValue(const Player& player) {
    int totalValue = 0;
    for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
        switch (player.hand[i].rank) {
        case SEVEN: totalValue += 7; break;
        case EIGHT: totalValue += 8; break;
        case NINE: totalValue += 9; break;
        case TEN: totalValue += 10; break;
        case JACK: totalValue += 10; break;
        case QUEEN: totalValue += 10; break;
        case KING: totalValue += 10; break;
        case ACE: totalValue += 11; break;
        }
    }
    return totalValue;
}

// Рунд на играта с показване на текущите баланси
void playRound(Player players[], int numPlayers, Card deck[], int& deckIndex) {
    int pot = 0;
    int lastRaise = 10;

    // Начално залагане
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].balance >= lastRaise) {
            players[i].balance -= lastRaise;
            pot += lastRaise;
        }
        else {
            players[i].isActive = false; // Ако нямат достатъчно пари, стават неактивни
        }
    }

//    while (true) { samo ako ima tie
        int activePlayers = 0;

        // Показване на баланси
        std::cout << "\nCurrent balances:\n";
        for (int i = 0; i < numPlayers; i++) {
            std::cout << "Player " << players[i].id << ": " << players[i].balance << " ";
        }
        std::cout << "\nPot: " << pot << "\n";

        for (int i = 0; i < numPlayers; i++) {
            if (!players[i].isActive || players[i].balance <= 0) continue;

            activePlayers++;

            std::cout << "\nPlayer " << players[i].id << "'s hand: ";
            std::cout << std::endl;
            printPlayerHand(players[i]);

            char action;
            bool validAction = false;

            while (!validAction) { // Цикъл, докато не се подаде валидно действие
                std::cout << "Player " << players[i].id << ", raise, call or fold? (r/c/f): ";
                std::cin >> action;

                if (action == 'r') {
                    validAction = true; // Валидно действие
                    int raiseAmount;
                    std::cout << "Enter raise amount: ";
                    std::cin >> raiseAmount;
                    if (raiseAmount < lastRaise) {
                        std::cout << "Raise must be at least " << lastRaise << "!\n";
                        validAction = false; // Отказ, връща към въпроса
                    }
                    else {
                        pot += raiseAmount;
                        players[i].balance -= raiseAmount;
                        lastRaise = raiseAmount;
                    }
                }
                else if (action == 'c') {
                    validAction = true; // Валидно действие
                    if (players[i].balance >= lastRaise) {
                        pot += lastRaise;
                        players[i].balance -= lastRaise;
                    }
                    else {
                        std::cout << "Player " << players[i].id << " cannot call and folds automatically.\n";
                        players[i].isActive = false;
                        activePlayers--;
                    }
                }
                else if (action == 'f') {
                    validAction = true; // Валидно действие
                    players[i].isActive = false;
                    activePlayers--;
                }
                else {
                    std::cout << "Invalid action. Please enter 'r', 'c', or 'f'.\n"; // Невалидно действие
                }
            }
        }

//        if (activePlayers <= 1) break; // Край на рунда, ако остане само един активен играч
//    }

    // Намиране на победителя
    int maxHandValue = 0;
    int winner = -1;

    for (int i = 0; i < numPlayers; i++) {
        if (!players[i].isActive) continue;

        int handValue = calculateHandValue(players[i]);
        std::cout << "Player " << players[i].id << " hand value: " << handValue << std::endl;

        if (handValue > maxHandValue) {
            maxHandValue = handValue;
            winner = i;
        }
    }

    if (winner != -1) {
        std::cout << "Player " << players[winner].id << " wins the pot of " << pot << "!\n";
        players[winner].balance += pot;
    }
    else {
        std::cout << "It's a tie! The pot carries over to the next round.\n";
    }

    // Възстановяване на активността на играчите за следващия рунд
    for (int i = 0; i < numPlayers; i++) {
        players[i].isActive = true;
    }
}

int initializeNewGame(Player players[]) {
    int numPlayers;
    std::cout << "Enter number of players (2-9): ";
    std::cin >> numPlayers;

    while (numPlayers < 2 || numPlayers > 9) {
        std::cout << "Invalid number of players! Enter again (2-9): ";
        std::cin >> numPlayers;
    }

    for (int i = 0; i < numPlayers; i++) {
        initializePlayer(players[i], i + 1, INITIAL_BALANCE);
    }
    return numPlayers;
}
void saveGameState(Player players[], int numPlayers) {
    std::ofstream outputFile("save_data.txt");
    if (!outputFile.is_open()) {
        std::cout << "Error saving game data!\n";
        return;
    }

    outputFile << numPlayers << std::endl;
    for (int i = 0; i < numPlayers; i++) {
        outputFile << players[i].balance << " ";
    }
    outputFile.close();
    std::cout << "Game data saved successfully.\n";
}


// Стартиране на играта
void startGame() {

    // if file != empty
    // continue?
    // if yes load players
    // if no init
    // else init

    int numPlayers = 0;
    char playAgain;
    Player players[MAX_PLAYERS];

    // Проверка дали файлът съдържа данни
    std::ifstream inputFile("save_data.txt");
    if (inputFile.is_open() && inputFile.peek() != std::ifstream::traits_type::eof()) {
        std::cout << "Previous game data found. Do you want to continue? (y/n): ";
        char continueGame;
        std::cin >> continueGame;

        if (continueGame == 'y') {
            inputFile >> numPlayers;
            for (int i = 0; i < numPlayers; i++) {
                inputFile >> players[i].balance;
                players[i].id = i + 1;
                players[i].isActive = (players[i].balance > 0);
            }
            inputFile.close();
        }
        else if (continueGame == 'n') {
            inputFile.close();
            std::ofstream clearFile("save_data.txt", std::ios::trunc); // Изчистване на файла
            clearFile.close();
            numPlayers = initializeNewGame(players);
        }
    }
    else {
        numPlayers = initializeNewGame(players);
    }

    // Създаване и разбъркване на тестето
    Card deck[DECK_SIZE];
    initializeDeck(deck, DECK_SIZE);
    shuffleDeck(deck, DECK_SIZE);

    int deckIndex = 0;

    do {
        dealCards(players, numPlayers, deck, deckIndex);
        playRound(players, numPlayers, deck, deckIndex);

        // Премахване на играчи с баланс <= 0
        int activePlayers = 0;
        for (int i = 0; i < numPlayers; i++) {
            if (players[i].balance > 0) {
                players[i].isActive = true;
                activePlayers++;
            }
            else {
                players[i].isActive = false;
            }
        }

        if (activePlayers == 0) {
            std::cout << "All players are out of balance! Game over.\n";
            break;
        }

        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;

        if (playAgain == 'y') {
            // Проверка дали има активни играчи
            shuffleDeck(deck, DECK_SIZE);
            deckIndex = 0;
        }
        else {
            saveGameState(players, numPlayers);
            break;
        }
    } while (playAgain == 'y');

    std::cout << "Game over. Thank you for playing!\n";

    // save data
    // close file
}
