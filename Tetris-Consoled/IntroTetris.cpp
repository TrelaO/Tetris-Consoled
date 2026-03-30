//IntroTetris.cpp

#include "IntroTetris.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

void printCentered(HANDLE hOut, int y, const std::string& text, int columns) {
    COORD pos;
    pos.X = (short)((columns - (int)text.length()) / 2);
    pos.Y = (short)y;
    SetConsoleCursorPosition(hOut, pos);
    std::cout << text;
}

void saveScore(const std::string& name, int score) {
    std::ofstream file("scores.txt", std::ios::app);
    if (file.is_open()) {
        file << name << " " << score << "\n";
        file.close();
    }
}

void showHighScores() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    printCentered(hOut, 2, "--- TOP 10 HIGH SCORES ---", columns);

    std::ifstream file("scores.txt");
    std::vector<std::pair<int, std::string>> scores;
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) {
            scores.push_back({ score, name });
        }
        file.close();
    }

    std::sort(scores.begin(), scores.end(), std::greater<std::pair<int, std::string>>());

    if (scores.empty()) {
        printCentered(hOut, rows / 2, "No scores recorded yet!", columns);
    }
    else {
        for (int i = 0; i < (int)scores.size() && i < 10; ++i) {
            std::string entry = std::to_string(i + 1) + ". " + scores[i].second + " - " + std::to_string(scores[i].first);
            printCentered(hOut, 5 + i, entry, columns);
        }
    }

    printCentered(hOut, rows - 3, "Press ENTER to return to Menu", columns);

    while (!(GetAsyncKeyState(VK_RETURN) & 0x8000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    while (GetAsyncKeyState(VK_RETURN) & 0x8000) std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void toggleFullscreen(bool fullscreen) {
    HWND hwnd = GetConsoleWindow();
    if (!hwnd) return;

    DWORD style = GetWindowLong(hwnd, GWL_STYLE);

    if (fullscreen) {
        SetWindowLong(hwnd, GWL_STYLE, style & ~(WS_CAPTION | WS_THICKFRAME));
        ShowWindow(hwnd, SW_MAXIMIZE);
    } else {
        SetWindowLong(hwnd, GWL_STYLE, style | WS_CAPTION | WS_THICKFRAME);
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    
    SetConsoleWindowInfo(hOut, TRUE, &csbi.srWindow);
}

Settings loadSettings() {
    Settings s;
    std::ifstream file("settings.txt");
    if (file.is_open()) {
        file >> s.ghostBlocks >> s.fullscreen;
        file.close();
    }
    return s;
}

void saveSettings(const Settings& s) {
    std::ofstream file("settings.txt");
    if (file.is_open()) {
        file << s.ghostBlocks << " " << s.fullscreen;
        file.close();
    }
}

void introTetris(Settings& settings) {

    std::vector<std::string> logoLines = {
        " _________ _______ _________ _______ _________ _______ ",
        "\\__   __/(  ____ \\\\__   __/(  ____ )\\__   __/(  ____ \\",
        "   ) (   | (    \\/   ) (   | (    )|   ) (   | (    \\/",
        "   | |   | (__       | |   | (____)|   | |   | (_____ ",
        "   | |   |  __)      | |   |     __)   | |   (_____  )",
        "   | |   | (         | |   | (\\ (      | |         ) |",
        "   | |   | (____/\\   | |   | ) \\ \\_____) (___/\\____) |",
        "   )_(   (_______/   )_(   |/   \\__/\\_______/\\_______)"
    };

    std::vector<std::string> instructions = {
        "Press ENTER to start game",
        "Press SHIFT for Options & Help",
        "Press H to see High Scores",
        "Press ESC to exit"
    };

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    bool showAnimation = true;

    while (true) {
        system("cls");
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        int startY = (rows / 2) - ((int)logoLines.size() / 2) - 4;

        for (size_t i = 0; i < logoLines.size(); ++i) {
            int x = (columns - (int)logoLines[i].length()) / 2;
            SetConsoleCursorPosition(hOut, { (short)x, (short)(startY + (int)i) });
            
            for (char c : logoLines[i]) {
                if (showAnimation) {
                    if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
                        showAnimation = false;
                    }
                }
                std::cout << c;
                if (showAnimation) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }

        while ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        showAnimation = false;

        int instrY = startY + (int)logoLines.size() + 2;
        for (int i = 0; i < instructions.size(); ++i) {
            printCentered(hOut, instrY + (i * 2), instructions[i], columns);
        }

        while (true) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000) return;
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) exit(0);
            if (GetAsyncKeyState('H') & 0x8000) { 
                showHighScores();
                break; 
            }
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    while (true) {
                    system("cls");
                    printCentered(hOut, 2, "--- OPTIONS & HELP ---", columns);
                    
                    std::string ghostStatus = settings.ghostBlocks ? "[ON]" : "[OFF]";
                    std::string fsStatus = settings.fullscreen ? "[ON]" : "[OFF]";
                    printCentered(hOut, 4, "1. Ghost Blocks: " + ghostStatus + " (Press '1' to toggle)", columns);
                    printCentered(hOut, 6, "2. Fullscreen: " + fsStatus + " (Press '2' to toggle)", columns);
                    
                    printCentered(hOut, 9, "--- CONTROLS ---", columns);
                    printCentered(hOut, 11, "Arrows / WSAD - Move & Rotate", columns);
                    printCentered(hOut, 13, "SPACE - Hard Drop", columns);
                    printCentered(hOut, 15, "Z / ESC - Pause", columns);
                    printCentered(hOut, 17, "C - Hold Block", columns);
                    printCentered(hOut, 21, "Press ENTER to save and return", columns);

                    bool redraw = false;
                    while (!redraw) {
                        if (GetAsyncKeyState('1') & 0x8000) {
                            settings.ghostBlocks = !settings.ghostBlocks;
                            saveSettings(settings);
                            std::this_thread::sleep_for(std::chrono::milliseconds(200));
                            redraw = true;
                        }
                        if (GetAsyncKeyState('2') & 0x8000) {
                            settings.fullscreen = !settings.fullscreen;
                            toggleFullscreen(settings.fullscreen);
                            saveSettings(settings);
                            std::this_thread::sleep_for(std::chrono::milliseconds(200));
                            redraw = true;
                        }
                        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                             while (GetAsyncKeyState(VK_RETURN) & 0x8000) std::this_thread::sleep_for(std::chrono::milliseconds(10));
                             goto exit_options; 
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    }
                }
                exit_options:
                break; 
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

void gameOver(int score) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int centerY = rows / 2;

    printCentered(hOut, centerY - 4, "GAME OVER", columns);
    printCentered(hOut, centerY - 2, "YOUR SCORE: " + std::to_string(score), columns);
    printCentered(hOut, centerY + 1, "Enter name to save (ESC to skip):", columns);

    COORD inputPos = { (short)(columns / 2 - 11), (short)(centerY + 3) };
    SetConsoleCursorPosition(hOut, inputPos);
    
    std::string playerName = "";
    bool skipSave = false;

    while (true) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) { skipSave = true; break; } 
            if (ch == 13) break;
            if (ch == 8 && !playerName.empty()) {
                playerName.pop_back();
                std::cout << "\b \b";
            } else if (isprint(ch) && playerName.length() < 20) {
                playerName += (char)ch;
                std::cout << (char)ch;
            }
        }
    }

    while (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (!skipSave && !playerName.empty()) {
        saveScore(playerName, score);
        printCentered(hOut, centerY + 5, "SCORE SAVED!", columns);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    system("cls");
    printCentered(hOut, centerY - 1, "Press ENTER for Menu", columns);
    printCentered(hOut, centerY + 1, "Press ESC to Exit", columns);

    while (true) {
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            while (GetAsyncKeyState(VK_RETURN) & 0x8000) std::this_thread::sleep_for(std::chrono::milliseconds(10));
            return;
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) exit(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}