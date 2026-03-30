//MainLoop.h

#pragma once

#include "Board.h"
#include "Block.h"
#include "IntroTetris.h"

/**
 * @brief Starts and manages the main game loop.
 * @param finalScore Reference to store the player's score after game over.
 * @param settings Current game configuration (ghost blocks, fullscreen).
 * @return true if player exited to menu, false if game ended with game over.
 */
bool mainLoop(int& finalScore, const Settings& settings);

/**
 * @brief Hides the console cursor to prevent flickering during rendering.
 */
void hideCursor();

/**
 * @brief Handles the game pause state, displaying the UI and waiting for player input.
 * @param board Reference to the board to allow redrawing after resume.
 * @param gamePaused Reference to the pause state flag.
 */
void pauseGame(Board& board, bool& gamePaused);