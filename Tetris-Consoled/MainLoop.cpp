#include "MainLoop.h"
#include "IntroTetris.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <chrono>
#include <windows.h>
#include <thread>

static void processInput(Board& board, Block& block, bool& playerMove, bool& gamePaused);

bool mainLoop() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    const int boardWidth = 10;
    const int boardHeight = 20;
    Board board(boardWidth, boardHeight);
    
    srand(static_cast<unsigned>(time(0)));
    Block::Type randomBlockType = static_cast<Block::Type>(rand() % 7);
    Block block(randomBlockType, board.getWidth() / 2 - 1, 0);
    
    hideCursor();
    board.drawBoard();
    bool gamePaused = false;

    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;

        bool playerMove = false;
        processInput(board, block, playerMove, gamePaused);
        
        if (gamePaused) {
            pauseGame(board, gamePaused);
            if (gamePaused) { 
                system("cls");
                return true; // WYJŚCIE DO MENU (ESC)
            }
            continue;
        }

        if (elapsed.count() >= 0.5) {
            board.removeBlock(block);

            block.moveDown();
            if (board.checkCollision(block)) {
                block.moveUp();
                board.lockBlock(block);
                board.clearWell();
                randomBlockType = static_cast<Block::Type>(rand() % 7);
                block = Block(randomBlockType, board.getWidth() / 2 - 1, 0);
                if (board.checkCollision(block)) {
                    return false; // GAME OVER (Przegrana)
                }
            }

            board.addBlock(block);
            lastTime = currentTime;
        }
        board.updateWell();
    }
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

static void processInput(Board& board, Block& block, bool& playerMove, bool& gamePaused) {
    if (_kbhit()) {
        int ch = _getch();
        
        // Obsługa ESC podczas gry - włącza pauzę
        if (ch == 27) {
            gamePaused = true;
            return;
        }

        // Obsługa klawiszy specjalnych (strzałki)
        if (ch == 0 || ch == 224) {
            ch = _getch(); 
            if (gamePaused) return;

            board.removeBlock(block);
            switch (ch) {
            case 75: // Strzałka w lewo
                block.moveLeft();
                if (board.checkCollision(block)) block.moveRight();
                playerMove = true;
                break;
            case 77: // Strzałka w prawo
                block.moveRight();
                if (board.checkCollision(block)) block.moveLeft();
                playerMove = true;
                break;
            case 80: // Strzałka w dół
                block.moveDown();
                if (board.checkCollision(block)) block.moveUp();
                playerMove = true;
                break;
            case 72: // Strzałka w górę (Obrót)
                goto rotate_logic;
                break;
            }
            board.addBlock(block);
            return;
        }

        if (ch == 'z' || ch == 'Z') {
            gamePaused = !gamePaused;
        }
        else if (!gamePaused) {
            board.removeBlock(block);
            switch (ch) {
            case 'a': case 'A':
                block.moveLeft();
                if (board.checkCollision(block)) block.moveRight();
                playerMove = true;
                break;
            case 'd': case 'D':
                block.moveRight();
                if (board.checkCollision(block)) block.moveLeft();
                playerMove = true;
                break;
            case 's': case 'S':
                block.moveDown();
                if (board.checkCollision(block)) block.moveUp();
                playerMove = true;
                break;
            case 'w': case 'W':
            rotate_logic:
                block.rotate();
                if (board.checkCollision(block)) {
                    bool rotatedSuccessfully = false;
                    for (int i = 0; i < 5; ++i) {
                        if (i == 0) block.moveRight();
                        else if (i == 1) block.moveLeft();
                        else if (i == 2) { block.moveLeft(); block.moveLeft(); }
                        else if (i == 3) { block.moveRight(); block.moveRight(); }
                        else if (i == 4) { block.moveUp(); }

                        if (!board.checkCollision(block)) {
                            rotatedSuccessfully = true;
                            break;
                        }

                        if (i == 0) block.moveLeft();
                        else if (i == 1) block.moveRight();
                        else if (i == 2) { block.moveRight(); block.moveRight(); }
                        else if (i == 3) { block.moveLeft(); block.moveLeft(); }
                        else if (i == 4) { block.moveDown(); }
                    }
                    if (!rotatedSuccessfully) block.rotate();
                }
                playerMove = true;
                break;
            }
            board.addBlock(block);
        }
    }
}

void pauseGame(Board& board, bool& gamePaused) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int pauseMessagePosX = (columns - 102);
    int pauseMessagePosY = (rows - 5);  

    COORD pos;
    pos.X = (short)pauseMessagePosX;
    pos.Y = (short)pauseMessagePosY;
    SetConsoleCursorPosition(consoleHandle, pos);

    std::cout << "Paused. 'Z' - Resume, 'ESC' - Main Menu, 'Q' - Quit Game";

    while (gamePaused) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) { // ESC
                return; // gamePaused pozostaje true -> mainLoop zwróci true -> powrót do intro
            }
            else if (ch == 'q' || ch == 'Q') {
                exit(0); // Całkowite zamknięcie aplikacji
            }
            else if (ch == 'z' || ch == 'Z') {
                gamePaused = false;  
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    system("cls");
    if (!gamePaused) board.drawBoard();
}