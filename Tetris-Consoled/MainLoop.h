#pragma once

#include "Board.h"
#include "Block.h"

/**
 * @brief Starts and manages the main game loop.
 */
void mainLoop();

/**
 * @brief Hides the console cursor to prevent flickering.
 */
void hideCursor();

/**
 * @brief Handles the game pause state and UI.
 */
void pauseGame(Board& board, bool& gamePaused);