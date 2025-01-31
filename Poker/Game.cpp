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

#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

// Инициализация на тестето
void initializeDeck(Card* deck, int size) {
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
void shuffleDeck(Card* deck, int DECK_SIZE) {
    srand(time(0));

    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Раздаване на карти
void dealCards(Player* players, int numPlayers, Card* deck, int& deckIndex) {

    if (deckIndex + (numPlayers * MAX_CARDS_IN_HAND) > DECK_SIZE) {
        shuffleDeck(deck, DECK_SIZE);
        deckIndex = 0;
    }

    for (int i = 0; i < numPlayers; i++) {
        if (!players[i].hand) {
            players[i].hand = new Card[MAX_CARDS_IN_HAND]; 
        }
        for (int j = 0; j < MAX_CARDS_IN_HAND; j++) {
            if (deckIndex < DECK_SIZE) {
                players[i].hand[j] = deck[deckIndex++];
            }
            else {
                std::cerr << "Error: Deck index out of bounds!\n";
                return;
            }
        }
    }
}

// Изчисляване на стойността на карта
int calculateCardValue(Rank rank) {

    switch (rank) {
    case ACE: return 11;
    case KING:
    case QUEEN:
    case JACK:
    case TEN: return 10;
    case NINE: return 9;
    case EIGHT: return 8;
    case SEVEN: return 7;
    default: return 0; // Невалидна карта
    }
}

// Изчисляване на различните комбинации
int calculateHandValue(const Player& player) {
    int totalValue = 0;
    bool hasClubSeven = false; // Проверка за наличието на 7♣
    int highestCardValue = 0;

    // Проверка за 7♣ в ръката
    for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
        if (player.hand[i].rank == SEVEN && player.hand[i].suit == CLUBS) {
            hasClubSeven = true;
        }
    }

    // Проверка за три еднакви карти
    if (player.hand[0].rank == player.hand[1].rank &&
        player.hand[1].rank == player.hand[2].rank) {
        if (player.hand[0].rank == SEVEN) {
            return 34; // Специален случай за три седмици
        }
        return calculateCardValue(player.hand[0].rank) * 3; // Три еднакви карти
    }

    // Проверка за карти от една боя
    if (player.hand[0].suit == player.hand[1].suit &&
        player.hand[1].suit == player.hand[2].suit) {
        for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
            totalValue += calculateCardValue(player.hand[i].rank);
        }
        return totalValue; // Сбор на стойностите при една боя
    }

    // Проверка за комбинация с две аса или две седмици
    if ((player.hand[0].rank == player.hand[1].rank && player.hand[0].rank == ACE) ||
        (player.hand[0].rank == player.hand[1].rank && player.hand[0].rank == SEVEN)) {
        return player.hand[0].rank == ACE ? 22 : 23; // Две аса или две седмици
    }

    // Проверка за комбинация със 7♣
    if (hasClubSeven) {
        int comboValue = 0;
        int highestCardValue = 0;
        int pairValue = 0;
        bool hasPair = false;

        for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
            int cardValue = calculateCardValue(player.hand[i].rank);
            if (cardValue > highestCardValue) {
                highestCardValue = cardValue;
            }
            comboValue += cardValue;

            // Проверка за чифт
            for (int j = i + 1; j < MAX_CARDS_IN_HAND; j++) {
                if (player.hand[i].rank == player.hand[j].rank) {
                    hasPair = true;
                    pairValue = calculateCardValue(player.hand[i].rank) * 2;
                }
            }
        }

        if (hasPair) {
            return pairValue + 11; // 7♣ + двойка
        }

        if (player.hand[0].suit == player.hand[1].suit || player.hand[1].suit == player.hand[2].suit) {
            return comboValue + 11; // 7♣ комбинирана с карти от една боя
        }
        return highestCardValue + 11; // 7♣ комбинирана с най-силната карта
    }

    // Ако няма комбинации, взима се стойността на най-високата карта
    for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
        int cardValue = calculateCardValue(player.hand[i].rank);
        if (cardValue > highestCardValue) {
            highestCardValue = cardValue;
        }
    }

    return highestCardValue; // Най-високата карта, ако няма комбинации
}

// Раздаване на карти на играчите в равенството
void dealCardsToActivePlayers(Player* players, int numPlayers, Card* deck, int& deckIndex) {
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].isActive) {
            for (int j = 0; j < MAX_CARDS_IN_HAND; ++j) {
                if (deckIndex < DECK_SIZE) {
                    players[i].hand[j] = deck[deckIndex++];
                }
                else {
                    std::cerr << "The deck is out of cards!" << std::endl;
                    return;
                }
            }
        }
    }
}

// Проверка за равенство
bool handleTie(Player* players, int numPlayers, int& pot, int maxHandValue, Card* deck) {
    int tiedPlayers[MAX_PLAYERS] = { 0 };
    int numTiedPlayers = 0;

    int tempPot = pot;
    int deckIndex = 0;

    // Избираме играчите с максималната ръка
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].isActive && calculateHandValue(players[i]) == maxHandValue) {
            tiedPlayers[numTiedPlayers++] = i;
            players[i].balance += CHIP_VALUE;
        }
    }

    // Проверка за равенство
    if (numTiedPlayers > 1) {
        std::cout << "\nIT IS A TIE\n";
        std::cout << "Players with equal hands:\n";
        for (int i = 0; i < numTiedPlayers; ++i) {
            std::cout << "Player " << players[tiedPlayers[i]].id << "\n";
        }

        bool allTiedPlayersBroke = true;
        for (int i = 0; i < numTiedPlayers; ++i) {
            if (players[tiedPlayers[i]].balance > 0) {
                allTiedPlayersBroke = false;
                break;
            }
        }

        bool alreadyInTie = false;

        if (allTiedPlayersBroke) {
            // Ако всички играчи в равенството нямат точки
            for (int i = 0; i < numTiedPlayers; ++i) {
                players[tiedPlayers[i]].balance = 50;
            }
            pot = 50 * numTiedPlayers;
            std::cout << "All tied players had zero balance. Each receives 50 points. New pot is " << pot << ".\n";
        }
        else {
            for (int i = 0; i < numPlayers; ++i) {
                for (int j = 0; j < numTiedPlayers; ++j) {
                    if (tiedPlayers[j] == i) {
                        alreadyInTie = true;
                        break;
                    }
                }
                if (alreadyInTie || !players[i].isActive) {
                    continue;
                }

                if (players[i].balance == 0) {
                    players[i].balance = 50;
                    players[i].isActive = true;
                    tiedPlayers[numTiedPlayers++] = i;
                    std::cout << "Player " << players[i].id
                        << " had zero balance and received 50 points to join the tie.\n";
                    continue;
                }
                else {
                    std::cout << "Player " << players[i].id
                        << ", do you want to join the TIE? You have to pay "
                        << tempPot / 2 << " (y/n): ";
                }

                char choice;
                std::cin >> choice;

                if (choice == 'y') {
                    players[i].balance -= tempPot / 2;
                    pot += tempPot / 2;
                    players[i].balance += CHIP_VALUE;
                    tiedPlayers[numTiedPlayers++] = i;
                    std::cout << "Player " << players[i].id << " has joined the tie!\n";
                }
                else {
                    std::cout << "Player " << players[i].id << " chose not to join.\n";
                }
            }
        }

        for (int i = 0; i < numPlayers; ++i) {
            players[i].isActive = false;
        }
        for (int i = 0; i < numTiedPlayers; ++i) {
            players[tiedPlayers[i]].isActive = true;
        }

        std::cout << "The pot carries over to the next round: " << pot << "\n";

        // Нов рунд
        shuffleDeck(deck, DECK_SIZE);
        dealCardsToActivePlayers(players, numTiedPlayers, deck, deckIndex);

        pot -= numTiedPlayers * CHIP_VALUE;

        playRound(players, numTiedPlayers, deck, pot);

        return true; 
    }

    return false;
}

// Премахване на играчи с нулев баланс
void removePlayersWithZeroBalance(Player*& players, int& numPlayers) {
    // Преброяваме играчите с ненулев баланс
    int activePlayersCount = 0;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].balance > 0) {
            ++activePlayersCount;
        }
    }

    if (activePlayersCount == 0) {
        delete[] players;
        players = nullptr;
        numPlayers = 0;
        return;
    }

    Player* activePlayers = new Player[activePlayersCount];
    int index = 0;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].balance > 0) {
            activePlayers[index++] = players[i];
        }
    }

    delete[] players;

    players = activePlayers;
    numPlayers = activePlayersCount;
}

// Инициализиране на игра
int initializeNewGame(Player*& players) {
    int numPlayers;
    std::cout << "How many players are going to play (2-9)?: ";
    std::cin >> numPlayers;

    while (numPlayers < 2 || numPlayers > 9) {
        std::cout << "Invalid number of players! Enter again (2-9): ";
        std::cin >> numPlayers;
    }

    players = new Player[numPlayers];
    for (int i = 0; i < numPlayers; i++) {
        initializePlayer(players[i], i + 1, INITIAL_BALANCE);
        players[i].hand = new Card[MAX_CARDS_IN_HAND]; 
    }
    return numPlayers;
}

// Протичането на рунда на играта
void playRound(Player* players, int numPlayers, Card* deck, int& pot) {
    int currentBet = 0; 
    int lastRaiseAmount = 0; 
    int raiserIndex = -1; 
    bool allCalled = false; 
    int activePlayers = numPlayers;

    // Начално залагане
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].balance >= CHIP_VALUE) {
            players[i].balance -= CHIP_VALUE;
            pot += CHIP_VALUE;
        }
        else {
            players[i].isActive = false; // Играчите без достатъчно средства стават неактивни
        }
    }

    // Показване на началния баланс и пот
    std::cout << "\nCurrent balances:\n";

    bool flag = true;

    for (int i = 0; i < numPlayers; i++) {
        std::cout << "Player " << players[i].id << ": " << players[i].balance << " ";
        
        //Подредба
        if (i == (numPlayers - 1) / 2 && flag) {
            std::cout << std::endl;
            flag = false;
        }
    }

    std::cout << std::endl;
    std::cout << "\nPot: " << pot << "\n";

    // Основен рунд
    bool* hasCalled = new bool[numPlayers](); // Запазва дали всеки играч е "call"-нал
    while (activePlayers > 1 && !allCalled) {
        allCalled = true; // Предполагаме, че всички са "call"-нали

        for (int i = 0; i < numPlayers; i++) {
            if (!players[i].isActive || players[i].balance <= 0 || hasCalled[i]) {
                continue; // Пропускаме неактивните играчи или тези, които са "call"-нали
            }

            std::cout << "\nPlayer " << players[i].id << "'s turn. Current bet to call: " << currentBet
                << "\nYour hand: " << std::endl;
            std::cout << std::endl;      
            printPlayerHand(&players[i]);

            char action;
            bool validAction = false;

            while (!validAction) {
                std::cout << "Player " << players[i].id << ", raise, call or fold? (r/c/f): ";
                std::cin >> action;

                if (action == 'r') { // Играчът вдига залога
                    validAction = true;
                    int raiseAmount;
                    std::cout << "Enter raise amount (minimum " << currentBet + CHIP_VALUE << "): ";
                    std::cin >> raiseAmount;

                    if (raiseAmount >= currentBet && raiseAmount <= players[i].balance) {
                        pot += raiseAmount - currentBet; // Добавяме разликата в пота
                        players[i].balance -= raiseAmount;
                        lastRaiseAmount = raiseAmount - currentBet;
                        currentBet += lastRaiseAmount;
                        raiserIndex = i; // Запомняме кой е "raise"-нал последен
                        allCalled = false; // Някой е "raise"-нал, значи не са "call"-нали всички
                        std::fill(hasCalled, hasCalled + numPlayers, false); // Зануляваме всички "call"-нали
                        hasCalled[i] = true; // Последният "raiser" е "call"-нал за себе си
                    }
                    else {
                        std::cout << "Invalid raise amount.\n";
                        validAction = false;
                    }
                }
                else if (action == 'c') { // Играчът колва
                    validAction = true;
                    if (players[i].balance >= currentBet) {
                        pot += currentBet;
                        players[i].balance -= currentBet;
                    }
                    else {
                        pot += players[i].balance; // Добавяме цялата останала сума на играча към пота
                        players[i].balance = 0; // Играчът остава без баланс
                        std::cout << "Player " << players[i].id << " is all in!\n";
                    }
                    hasCalled[i] = true; // Маркираме, че този играч е "call"-нал
                }
                else if (action == 'f') { // Играчът се отказва
                    validAction = true;
                    players[i].isActive = false;
                    activePlayers--;
                    std::cout << "Player " << players[i].id << " folds.\n";

                    if (activePlayers == 1) {
                        for (int j = 0; j < numPlayers; j++) {
                            if (players[j].isActive) {
                                std::cout << "Player " << players[j].id << " wins the pot of " << pot << "!\n";
                                players[j].balance += pot;
                                return; 
                            }
                        }
                    }
                }
                else {
                    std::cout << "Invalid action. Please enter 'r', 'c', or 'f'.\n";
                }
            }
        }

        // Проверка дали всички са "call"-нали или "fold"-нали
        allCalled = true;
        for (int i = 0; i < numPlayers; i++) {
            if (players[i].isActive && !hasCalled[i]) {
                allCalled = false;
                break;
            }
        }
    }

    // След края на рунда, намираме победителя или обработваме равенството
    int winner = -1;
    int maxHandValue = 0;

    for (int i = 0; i < numPlayers; i++) {
        if (players[i].isActive) {
            int handValue = calculateHandValue(players[i]);
            if (handValue > maxHandValue) {
                maxHandValue = handValue;
                winner = i;
            }
        }
    }

    // Проверка за равенство
    if (handleTie(players, numPlayers, pot, maxHandValue, deck)) {
        return; // Ако има равенство, излизаме от рунда
    }

    // Награждаваме победителя
    if (winner != -1) {
        std::cout << std::endl;
        std::cout << "Player " << players[winner].id << " wins the pot of " << pot << "!\n";
        players[winner].balance += pot;
    }

    std::cout << "\nPoints after this round:\n";
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].isActive) {
            int handValue = calculateHandValue(players[i]);
            std::cout << "Player " << players[i].id << ": " << handValue << " points\n";
        }
        else {
            std::cout << "Player " << players[i].id << ": Folded\n";
        }
    }

    // Показваме актуалните баланси след рунда
    std::cout << "\nUpdated balances:\n";
    for (int i = 0; i < numPlayers; i++) {
        std::cout << "Player " << players[i].id << ": " << players[i].balance << " ";
    }
    std::cout << std::endl;

    // Възстановяване на активността на играчите за следващия рунд
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].balance > 0) {
            players[i].isActive = true;
        }
        else {
            players[i].isActive = false;
        }
    }
}

// Работа с файлове
void fileManage(int& numPlayers, Player*& players) {
    std::ifstream inputFile("save_data.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open save_data.txt. Starting a new game.\n";
        numPlayers = initializeNewGame(players);
        return;
    }

    if (inputFile.peek() == std::ifstream::traits_type::eof()) {
        numPlayers = initializeNewGame(players);
        inputFile.close();
        return;
    }

    char continueGame;
    std::cout << "Previous game data found. Do you want to continue? (y/n): ";
    std::cin >> continueGame;

    if (continueGame == 'y') {
        if (!(inputFile >> numPlayers) || numPlayers < 2 || numPlayers > 9) {
            std::cerr << "Invalid data in save_data.txt. Starting a new game.\n";
            numPlayers = initializeNewGame(players);
        }
        else {
            players = new Player[numPlayers];
            for (int i = 0; i < numPlayers; ++i) {
                if (!(inputFile >> players[i].id >> players[i].balance)) {
                    std::cerr << "Error: Invalid player data in save_data.txt. Starting a new game.\n";
                    delete[] players;
                    players = nullptr;
                    numPlayers = initializeNewGame(players);
                    break;
                }
                players[i].isActive = (players[i].balance > 0);
                players[i].hand = new Card[MAX_CARDS_IN_HAND]; // Инициализиране на ръката
            }
        }
    }
    else {
        numPlayers = initializeNewGame(players);
    }

    inputFile.close();
}

// Запазване на информация за играта
void saveGameState(Player* players, int numPlayers) {
    std::ofstream outputFile("./save_data.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file to save game data!\n";
        return;
    }

    // Запазваме броя на играчите
    outputFile << numPlayers << "\n";

    // Запазваме данните за всеки играч
    for (int i = 0; i < numPlayers; i++) {
        outputFile << players[i].id << " " << players[i].balance << "\n";
    }

    outputFile.close();
    std::cout << "Game data saved successfully.\n";
}

// Стартиране на играта
void startGame() {
    Player* players = nullptr;
    Card* deck = new Card[DECK_SIZE];
    int* deckIndex = new int(0);
    int pot = 0;

    int numPlayers = 0;
    fileManage(numPlayers, players);

    initializeDeck(deck, DECK_SIZE);
    shuffleDeck(deck, DECK_SIZE);

    char playAgain;

    do {
        // Премахване на играчи с нулев баланс
        removePlayersWithZeroBalance(players, numPlayers);

        // Check if only one player remains
        if (numPlayers == 1) {
            std::cout << "Player " << players[0].id << " is the last player standing and wins the game!\n";
            break;
        }

        // Check if there are no players left (shouldn't happen logically but good to safeguard)
        if (numPlayers == 0) {
            std::cout << "No players left in the game. Game over.\n";
            break;
        }

        dealCards(players, numPlayers, deck, *deckIndex);
        playRound(players, numPlayers, deck, pot);
        
        std::cout << std::endl;
        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;

        if (playAgain == 'y') {
            pot = 0;
            shuffleDeck(deck, DECK_SIZE);
            *deckIndex = 0;
        } 
        else if (playAgain == 'n') {
            break;
        } 
        else {
            std::cout << "Invalid input.\n";
        }
    } while (playAgain == 'y');

    std::cout << "Game over. Thank you for playing!\n";

    saveGameState(players, numPlayers);

    // Изчистване на димачината памет
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].hand) {
            delete[] players[i].hand; 
            players[i].hand = nullptr; 
        }
    }
    delete[] players; 
    players = nullptr;

    delete[] deck;
    deck = nullptr;

    delete deckIndex;
    deckIndex = nullptr;
}