//Tetris.cpp﻿

#include "MainLoop.h"
#include "IntroTetris.h"

int main() {
    while (true) {
        introTetris(); 
        int score = 0;
        bool exitedToMenu = mainLoop(score);
        
        if (!exitedToMenu) {
            gameOver(score); 
        }
    }
    return 0;
}