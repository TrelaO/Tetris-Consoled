#pragma once
#include "Block.h"
#include <vector>

class Board {
private:
    const int boardWidth;
    const int boardHeight;
    std::vector<std::vector<int>> well;

public:
    Board(int w, int h);

    void addBlock(const Block& block);
    void removeBlock(const Block& block);
    void drawBoard() const;
    void clearWell();
    void updateWell();
    bool checkCollision(const Block& block) const;
    void lockBlock(const Block& block);

    int getWidth() const { return boardWidth; }
    int getHeight() const { return boardHeight; }
};