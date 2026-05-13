<div align="center">

# 🃏 Alternative Poker

### A console-based custom poker variant in C++ with a 32-card deck, 3-card hands, and a hand-ranking system of its own.

![C++](https://img.shields.io/badge/C%2B%2B-MSVC%20v143-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Course](https://img.shields.io/badge/SU%20FMI-Intro%20to%20Programming-yellow?style=for-the-badge)

<img src="https://github.com/user-attachments/assets/109a8248-178f-459c-8f51-97cf076abaa4" alt="Alternative Poker Banner" width="600"/>

</div>

---

## 📖 About

**Alternative Poker** is a custom poker variant implemented as a console game in C++. It uses a stripped-down **32-card deck** (7 → Ace in four suits), deals every player a **3-card hand**, and ranks those hands with its own table of combinations rather than standard poker rankings.

Built as **course project #10** for the *Introduction to Programming* course at the **Faculty of Mathematics and Informatics, Sofia University** (Winter semester 2024/2025), the project follows a **procedural C++** style — `struct Player`, `struct Card`, free functions, manual `new` / `delete`, and direct file I/O. No classes, no inheritance, no STL containers.

---

## ✨ Features

- 🃏 **32-card deck** — ranks 7, 8, 9, 10, J, Q, K, A in all four suits (Clubs, Diamonds, Hearts, Spades)
- 👥 **2 – 9 players** in a single session, each starting with **1000 points** (100 chips × 10)
- ✋ **3 cards per hand**
- 💰 **Standard betting actions** — `r` raise, `c` call, `f` fold
- 🪙 **Forced initial bet** of one chip (10 points) per player at the start of each round
- 🤝 **Tie handling** — players with the best hand split the pot; other active players may pay half the pot to buy into the showdown; if every tied player is broke they each get 50 free points and a fresh tie round
- 💀 **All-in mechanic** — if a player's balance is less than the current bet, the whole balance goes to the pot
- 🗂️ **Persistent state** — game progress is auto-saved to `save_data.txt` and loaded on the next launch
- 🏆 **Last-player-standing wins** — once everyone else is out of points the game ends

---

## 🎰 Hand Ranking System

This is what makes the game *alternative*. Hands are scored in **points**, not by classic poker rank — higher points win the pot. Rules are checked in this order:

| Combination | Score |
|-------------|-------|
| **Three sevens** (7♠ 7♥ 7♦ 7♣, any three) | **34** *(special case)* |
| **Three of a kind** (any other rank) | card value × 3 |
| **Flush** (three cards of the same suit) | sum of the three card values |
| **Pair of Aces** | 22 |
| **Pair of Sevens** | 23 |
| **Hand contains 7♣ + a pair** | pair value × 2 + 11 |
| **Hand contains 7♣ + partial flush** | sum of card values + 11 |
| **Hand contains 7♣** (no other combo) | highest card value + 11 |
| **No combination** | value of the highest card |

### Card values

| Rank | 7 | 8 | 9 | 10 | J | Q | K | A |
|------|---|---|---|---|---|---|---|---|
| Points | 7 | 8 | 9 | 10 | 10 | 10 | 10 | 11 |

The **7 of Clubs (7♣)** is a special card — being dealt it gives you a flat **+11 bonus** that combines with other partial combinations.

---

## 🛠️ Tech Stack

| Layer       | Choice                            |
|-------------|-----------------------------------|
| Language    | C++ (procedural style, MSVC v143) |
| Build       | MSBuild (`.sln` / `.vcxproj`)     |
| IDE         | Visual Studio 2022                |
| Platform    | Windows (x86 & x64)               |
| Persistence | Plain text file (`save_data.txt`) |

---

## 📁 Project Structure

```
Alternative-Poker/
├── Poker/
│   ├── main.cpp                       # Entry point — just calls startGame()
│   ├── Game.{h,cpp}                   # Deck, dealing, betting, ties, save/load, main loop
│   ├── Player.{h,cpp}                 # Player struct + helpers
│   ├── Card.{h,cpp}                   # Card struct + printing helpers
│   ├── save_data.txt                  # Persistent game state (player IDs + balances)
│   ├── Alternative-Poker.sln          # Visual Studio solution
│   └── Alternative-Poker.vcxproj      # Visual Studio project
├── .gitignore
└── README.md
```

> Note: the solution file lives **inside** the `Poker/` folder, not at the repository root.

---

## 🚀 Build & Run

### Prerequisites

- **Windows 10 / 11**
- **Visual Studio 2022** with the *Desktop development with C++* workload

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/LubomirKrastev/Alternative-Poker.git
   cd Alternative-Poker/Poker
   ```
2. Open `Alternative-Poker.sln` in Visual Studio 2022.
3. Pick a configuration (**Debug** or **Release**) and a platform.
4. Press **F5** to build and run, or **Ctrl + Shift + B** to build only.

> 💡 Command-line build with MSBuild:
> ```bash
> msbuild Alternative-Poker.sln /p:Configuration=Release /p:Platform=x64
> ```

---

## 🎮 How to Play

When you launch the executable:

1. **If a previous game is found** in `save_data.txt`, you'll be asked:
   ```
   Previous game data found. Do you want to continue? (y/n):
   ```
   Answer `y` to resume, `n` to start fresh.

2. **For a new game**, the program asks how many players (2 – 9) — each is automatically assigned an ID and a starting balance of 1000 points.

3. **Every round** runs like this:
   - A forced chip (10 points) is collected from each active player into the pot.
   - Each player is dealt 3 cards and their hand is printed.
   - On their turn, every player chooses:
     - `r` — **raise** (you'll be prompted for the new bet)
     - `c` — **call** (match the current bet)
     - `f` — **fold** (drop out of the round)
   - Betting continues until everyone still in the round has either called the current bet or folded.
   - If only one player remains, they take the pot. Otherwise, hand values are compared.
   - Tie? Players with equal top scores split. Other active players are offered to **buy into the tie** by paying half the pot. If everyone in the tie is broke, each gets 50 points and a fresh tie round is dealt.

4. **After the round** you're asked:
   ```
   Play again? (y/n):
   ```

5. **Game over** when only one player has a positive balance, or you quit. On exit, the state is saved automatically.

---

## 👤 Author

**Lyubomir Krastev**

- GitHub: [@LubomirKrastev](https://github.com/LubomirKrastev)
- Faculty Number: `1MI0600441`
- Course: *Introduction to Programming*, FMI – Sofia University, Winter 2024 / 2025

---

## 📄 Notes

This repository is a coursework submission and is shared for educational reference. The "alternative" rules are a custom assignment specification, not standard poker.
