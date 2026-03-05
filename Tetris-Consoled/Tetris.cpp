//Tetris.cpp﻿

#include "MainLoop.h"
#include "IntroTetris.h"

int main() {
    while (true) {
        introTetris(); 
        bool exitedToMenu = mainLoop();
        
        if (!exitedToMenu) {
            gameOver(); 
        }
    }
    return 0;
}