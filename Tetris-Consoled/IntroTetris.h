#pragma once
#include <string>

struct Settings {
    bool ghostBlocks = true;
};

Settings loadSettings();
void saveSettings(const Settings& s);
void introTetris(Settings& settings); // Zmieniamy sygnaturę
void gameOver(int score);