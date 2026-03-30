//IntroTetris.h

#pragma once
#include <string>

/**
 * @brief Structure holding persistent game configuration.
 */
struct Settings {
    bool ghostBlocks = true; /**< Toggle for displaying where the block will land. */
    bool fullscreen = false; /**< Toggle for windowed/borderless maximization. */
};

/** @brief Loads settings from the local storage file. */
Settings loadSettings();

/** @brief Saves current settings to the local storage file. */
void saveSettings(const Settings& s);

/** @brief Displays the main menu, splash screen and handles settings navigation. */
void introTetris(Settings& settings);

/** @brief Toggles between windowed mode and borderless maximization. */
void toggleFullscreen(bool fullscreen);

/** @brief Reads high scores from file and displays the leaderboard. */
void showHighScores();

/** @brief Displays game over screen, handles high scores and returns to menu. */
void gameOver(int score);