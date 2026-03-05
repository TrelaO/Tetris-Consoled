#include "IntroTetris.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include <string>

void introTetris() {
    std::string title = "TETRIS";
    std::string instructions[] = {
        "Press ENTER to start game",
        "Press SHIFT for help",
        "Press ESC to exit"
    };
    const int animationDelay = 100;

    bool showAnimation = true;

    while (true) {
        system("cls");

        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(consoleHandle, &csbi);
        int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        int titleStartPos = (columns - (int)title.length()) / 2;
        int instructionsStartPos[3];
        for (int i = 0; i < 3; ++i) {
            instructionsStartPos[i] = (columns - (int)instructions[i].length()) / 2;
        }

        COORD pos;
        pos.X = (short)titleStartPos;
        pos.Y = (short)(rows / 2 - 3);
        SetConsoleCursorPosition(consoleHandle, pos);

        if (showAnimation) {
            for (char c : title) {
                std::cout << c;
                std::this_thread::sleep_for(std::chrono::milliseconds(animationDelay));
            }
            showAnimation = false;
        } else {
            std::cout << title;
        }

        for (int i = 0; i < 3; ++i) {
            pos.X = (short)instructionsStartPos[i];
            pos.Y += 2;
            SetConsoleCursorPosition(consoleHandle, pos);
            std::cout << instructions[i] << std::endl;
        }

        bool actionTaken = false;
        while (!actionTaken) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                system("cls");
                return; 
            }
            else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                exit(0); 
            }
            else if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                system("cls");
                std::cout << "Help:\n";
                std::cout << "Use arrow keys or WSAD to move/rotate.\n";
                std::cout << "Press Z or ESC to pause the game.\n";
                std::cout << "In Pause: ESC - back to Menu, Q - Quit Game.\n";
                std::cout << "\nPress ENTER to return to menu.";

                while (!(GetAsyncKeyState(VK_RETURN) & 0x8000)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                while (GetAsyncKeyState(VK_RETURN) & 0x8000) {} 
                actionTaken = true; 
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

void gameOver() {
    std::string gameOverText = "GAME OVER";
    std::string instructions[] = {
        "Press ENTER to return to Menu",
        "Press ESC to exit game"
    };

    system("cls");

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int gameOverStartPos = (columns - (int)gameOverText.length()) / 2;
    int instrStartPos[2];
    for (int i = 0; i < 2; ++i) {
        instrStartPos[i] = (columns - (int)instructions[i].length()) / 2;
    }

    COORD pos;
    pos.X = (short)gameOverStartPos;
    pos.Y = (short)(rows / 2 - 2);
    SetConsoleCursorPosition(consoleHandle, pos);

    for (char c : gameOverText) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (int i = 0; i < 2; ++i) {
        pos.X = (short)instrStartPos[i];
        pos.Y += 2;
        SetConsoleCursorPosition(consoleHandle, pos);
        std::cout << instructions[i] << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    while (GetAsyncKeyState(VK_RETURN) & 0x8000) {}

    while (true) {
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            system("cls");
            return; 
        }
        else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            exit(0); 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}