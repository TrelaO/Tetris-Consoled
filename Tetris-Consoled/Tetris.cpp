//Tetris.cpp﻿

#include "MainLoop.h"
#include "IntroTetris.h"

int main() {
    Settings settings = loadSettings(); 
    while (true) {
        introTetris(settings); 
        int score = 0;
        bool exitedToMenu = mainLoop(score); 
        
        if (!exitedToMenu) {
            gameOver(score); 
        }
    }
    return 0;
}





