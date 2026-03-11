//Board.h

#pragma once

#include "Block.h"
#include <vector>
#include <string>

class Board {
private:
    const int boardWidth;
    const int boardHeight;
    std::vector<std::vector<int>> well;
    std::vector<std::vector<std::string>> displayBuffer; 


    mutable Block::Type lastNextType;
    mutable Block::Type lastHoldType;
    mutable bool lastHasHold = false;
    mutable int lastLevel = 0;
    mutable bool firstDraw = true;

public:
    Board(int w, int h);

    void addBlock(const Block& block);
    void removeBlock(const Block& block);
    void drawBoard(int level, int score) const; 
    void drawGhostBlock(const Block& block);
    int clearWell();
    void updateWell(const Block& currentBlock, Block::Type nextType, Block::Type holdType, bool hasHold, int level, int score, bool showGhost);
    bool checkCollision(const Block& block) const;
    void lockBlock(const Block& block);

    int getWidth() const { return boardWidth; }
    int getHeight() const { return boardHeight; }
};

