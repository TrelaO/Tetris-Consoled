//Board.h

#pragma once

#include "Block.h"
#include <vector>
#include <string>

/**
 * @brief Manages the game board, collision detection, and rendering logic.
 * Handles the "well" where blocks are locked and displays the HUD.
 */
class Board {
private:
    int offsetX;    /**< Dynamic horizontal rendering margin for centering the board. */
    int offsetY;    /**< Dynamic vertical rendering margin for centering the board. */
    int boardDrawX; /**< Calculated X coordinate for the board's interior frame. */
    int hudX;       /**< Calculated X coordinate for the side HUD (Next/Hold/Score) area. */

    int lastCols = 0; /**< Cached column count of the window, used to detect manual resizing. */
    int lastRows = 0; /**< Cached row count of the window, used to detect manual resizing. */

    int boardWidth;  /**< Number of horizontal cells (columns) in the well. */
    int boardHeight; /**< Number of vertical cells (rows) in the well. */

    std::vector<std::vector<int>> well; /**< 2D grid storing locked blocks (0: empty, 1: active, 2: locked). */
    std::vector<std::vector<std::string>> displayBuffer; /**< Graphical buffer tracking symbols currently drawn on screen. */

    mutable Block::Type lastNextType; /**< Cache for the preview block to avoid unnecessary redraws. */
    mutable Block::Type lastHoldType; /**< Cache for the held block to avoid unnecessary redraws. */
    mutable bool lastHasHold = false; /**< Cache for the hold status. */
    mutable int lastLevel = 0;        /**< Cache for level display. */
    mutable bool firstDraw = true;    /**< Flag to force a full board redraw (e.g., after resize or pause). */

public:
    /**
     * @brief Initializes the board with specific dimensions and calculates initial centering.
     * @param w Desired well width in cells.
     * @param h Desired well height in cells.
     */
    Board(int w, int h);

    /**
     * @brief Sets the active block's points in the well to 1 (active state).
     * @param block The block instance to add.
     */
    void addBlock(const Block& block);

    /**
     * @brief Clears the active block's points from the well (resets to 0).
     * @param block The block instance to remove.
     */
    void removeBlock(const Block& block);

    /**
     * @brief Performs an initial, full redraw of the board frame, boundaries, and HUD labels.
     * @param level Current game level to display.
     * @param score Current game score to display.
     */
    void drawBoard(int level, int score) const;

    /**
     * @brief Draws the "ghost" landing preview for the current block at its lowest possible position.
     * @param block Reference to the current active block.
     */
    void drawGhostBlock(const Block& block);

    /**
     * @brief Checks for full rows, clears them, and shifts remaining blocks down.
     * @return Total number of lines cleared in this call.
     */
    int clearWell();

    /**
     * @brief Updates changed cells on the screen and manages side HUD updates.
     * @param currentBlock The active tetromino.
     * @param nextType Type of the upcoming tetromino.
     * @param holdType Type of the piece currently in hold.
     * @param hasHold Flag indicating if a piece is actually held.
     * @param level Current level for HUD update.
     * @param score Current score for HUD update.
     * @param showGhost Toggle for rendering the landing preview.
     */
    void updateWell(const Block& currentBlock, Block::Type nextType, Block::Type holdType, bool hasHold, int level, int score, bool showGhost);

    /**
     * @brief Validates if the block's current points overlap with locked blocks or board walls.
     * @param block The tetromino to check.
     * @return true if there is an overlap or out-of-bounds, false otherwise.
     */
    bool checkCollision(const Block& block) const;

    /**
     * @brief Permanently integrates the block's points into the well as locked cells (state 2).
     * @param block The tetromino to lock.
     */
    void lockBlock(const Block& block);

    /**
     * @brief Detects window size changes and recalculates centering offsets to keep the UI centered.
     * @param level Current level (passed to drawBoard if resize is detected).
     * @param score Current score (passed to drawBoard if resize is detected).
     */
    void recalculateOffsets(int level, int score);

    /** @brief Gets the well's width in cells. */
    int getWidth() const { return boardWidth; }

    /** @brief Gets the well's height in cells. */
    int getHeight() const { return boardHeight; }
};