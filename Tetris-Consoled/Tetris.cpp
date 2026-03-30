//Tetris.cpp﻿

#include "MainLoop.h"
#include "IntroTetris.h"

int main() {
    Settings settings = loadSettings(); 
    toggleFullscreen(settings.fullscreen);
    while (true) {
        introTetris(settings); 
        int score = 0;
        bool exitedToMenu = mainLoop(score, settings); 
        
        if (!exitedToMenu) {
            gameOver(score); 
        }
    }
    return 0;
}





