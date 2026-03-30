//Board.cpp

#include "Board.h"
#include <iostream>
#include <windows.h>


void Board::recalculateOffsets(int level, int score) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

    int newColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int newRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (newColumns == this->lastCols && newRows == this->lastRows) return;

    this->lastCols = newColumns;
    this->lastRows = newRows;

    int totalWidth = (boardWidth * 2) + 25;
    offsetX = (newColumns - totalWidth) / 2;
    offsetY = (newRows - boardHeight) / 2;

    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;

    boardDrawX = offsetX + 2;
    hudX = offsetX + (boardWidth * 2) + 8;

    
    system("cls");      
    firstDraw = true;   
    drawBoard(lastLevel, 0); 
}

Board::Board(int w, int h) 
    : boardWidth(w), boardHeight(h), 
      well(w, std::vector<int>(h, 0)), 
      displayBuffer(w, std::vector<std::string>(h, "  ")) 
{
    int level = 0; 
    int score = 0;
    recalculateOffsets(level, score); 
}

void Board::addBlock(const Block& block) {
    for (const auto& point : block.getPoints()) {
        if (point.x >= 0 && point.x < boardWidth && point.y >= 0 && point.y < boardHeight) {
            well[point.x][point.y] = 1;
        }
    }
}

void Board::removeBlock(const Block& block) {
    for (const auto& point : block.getPoints()) {
        if (point.x >= 0 && point.x < boardWidth && point.y >= 0 && point.y < boardHeight) {
            well[point.x][point.y] = 0;
        }
    }
}

void Board::drawBoard(int level, int score) const {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    firstDraw = true; 
    lastLevel = level;


    for (int i = 0; i < boardHeight; ++i) {
        SetConsoleCursorPosition(hOut, { (short)offsetX, (short)(offsetY + i) });
        std::cout << "##";
        SetConsoleCursorPosition(hOut, { (short)(offsetX + 2 + (boardWidth * 2)), (short)(offsetY + i) });
        std::cout << "## " << (boardHeight - i);
    }
    SetConsoleCursorPosition(hOut, { (short)offsetX, (short)(offsetY + boardHeight) });
    for (int i = 0; i < boardWidth + 2; ++i) std::cout << "##";


    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 1) });
    std::cout << "NEXT BLOCK";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 2) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 7) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 10) });
    std::cout << "HOLD BLOCK";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 11) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 16) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 18) });
    std::cout << "LEVEL: " << level;
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 19) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)hudX, (short)(offsetY + 20) });
    std::cout << "SCORE: " << score;
}

int Board::clearWell() {
    int linesCleared = 0;
    for (int i = 0; i < boardHeight; ++i) {
        bool fullLine = true;
        for (int j = 0; j < boardWidth; ++j) {
            if (well[j][i] != 2) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            linesCleared++;
            for (int k = i; k > 0; --k) {
                for (int j = 0; j < boardWidth; ++j) {
                    well[j][k] = well[j][k - 1];
                }
            }
            for (int j = 0; j < boardWidth; ++j) well[j][0] = 0;
        }
    }
    return linesCleared;
}

static void drawBlockPreview(Block::Type type, int startX, int startY, const std::string& symbol) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < 4; ++i) {
        SetConsoleCursorPosition(hOut, { (short)startX, (short)(startY + i) });
        std::cout << "        ";
    }

    std::vector<std::pair<int, int>> shape;
    switch (type) {
        case Block::Type::I: shape = { {1,0},{1,1},{1,2},{1,3} }; break;
        case Block::Type::J: shape = { {1,0},{1,1},{1,2},{0,2} }; break;
        case Block::Type::L: shape = { {1,0},{1,1},{1,2},{2,2} }; break;
        case Block::Type::O: shape = { {1,1},{2,1},{1,2},{2,2} }; break;
        case Block::Type::S: shape = { {2,1},{1,1},{1,2},{0,2} }; break;
        case Block::Type::T: shape = { {1,1},{0,2},{1,2},{2,2} }; break;
        case Block::Type::Z: shape = { {0,1},{1,1},{1,2},{2,2} }; break;
    }

    for (auto& p : shape) {
        SetConsoleCursorPosition(hOut, { (short)(startX + p.first * 2), (short)(startY + p.second) });
        std::cout << symbol;
    }
}

void Board::updateWell(const Block& currentBlock, Block::Type nextType, Block::Type holdType, bool hasHold, int level, int score, bool showGhost) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    std::vector<Point> ghostPoints;
    if (showGhost) {
        Block ghost = currentBlock;
        removeBlock(ghost);
        while (!checkCollision(ghost)) {
            ghost.moveDown();
        }
        ghost.moveUp();
        addBlock(currentBlock);
        ghostPoints = ghost.getPoints();
    }

    for (int i = 0; i < boardHeight; ++i) {
        for (int j = 0; j < boardWidth; ++j) {
            std::string symbol = "  ";

            if (this->well[j][i] == 1) symbol = "()";      
            else if (well[j][i] == 2) symbol = "[]"; 
            else {
                for (const auto& p : ghostPoints) {
                    if (p.x == j && p.y == i) {
                        symbol = "::";
                        break;
                    }
                }
            }

            if (symbol != displayBuffer[j][i] || firstDraw) {
                SetConsoleCursorPosition(hOut, { (short)(boardDrawX + (j * 2)), (short)(offsetY + i) });
                std::cout << symbol;
                displayBuffer[j][i] = symbol;
            }
        }
    }


    if (firstDraw || nextType != lastNextType) {
        drawBlockPreview(nextType, hudX + 1, offsetY + 3, "()");
        lastNextType = nextType;
    }

    if (hasHold && (firstDraw || !lastHasHold || holdType != lastHoldType)) {
        drawBlockPreview(holdType, hudX + 1, offsetY + 12, "[]");
        lastHoldType = holdType;
        lastHasHold = true;
    }

    if (firstDraw || level != lastLevel) {
        SetConsoleCursorPosition(hOut, { (short)(hudX + 7), (short)(offsetY + 18) });
        std::cout << level << "  "; 
        lastLevel = level;
    }
    
    static int lastScore = -1;
    if (firstDraw || score != lastScore) {
        SetConsoleCursorPosition(hOut, { (short)(hudX + 7), (short)(offsetY + 20) });
        std::cout << score << "       "; 
        lastScore = score;
    }

    firstDraw = false;
}

void Board::lockBlock(const Block& block) {
    for (const auto& point : block.getPoints()) {
        if (point.x >= 0 && point.x < boardWidth && point.y >= 0 && point.y < boardHeight) {
            well[point.x][point.y] = 2;
        }
    }
}

bool Board::checkCollision(const Block& block) const {
    for (const auto& point : block.getPoints()) {
        if (point.x < 0 || point.x >= boardWidth || point.y < 0 || point.y >= boardHeight || well[point.x][point.y] == 2) {
            return true;
        }
    }
    return false;
}

void Board::drawGhostBlock(const Block& block) {
    Block ghost = block;
    
    while (!checkCollision(ghost)) {
        ghost.moveDown();
    }
    ghost.moveUp(); 

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (const auto& point : ghost.getPoints()) {
        if (point.x >= 0 && point.x < boardWidth && point.y >= 0 && point.y < boardHeight) {
            if (well[point.x][point.y] == 0) {
                short consoleX = (short)(boardDrawX + (point.x * 2));
                short consoleY = (short)(offsetY + point.y);
                
                SetConsoleCursorPosition(hOut, { consoleX, consoleY });
                std::cout << "::"; 
            }
        }
    }
}