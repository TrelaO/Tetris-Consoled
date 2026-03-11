//Board.cpp

#include "Board.h"
#include <iostream>
#include <windows.h>


const int OFFSET_X = 20;    
const int OFFSET_Y = 2;     
const int BOARD_DRAW_X = OFFSET_X + 2; 
const int HUD_X = OFFSET_X + 29;       

Board::Board(int w, int h) 
    : boardWidth(w), boardHeight(h), well(w, std::vector<int>(h, 0)), 
      lastNextType(Block::Type::I), lastHoldType(Block::Type::I) {}

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
        SetConsoleCursorPosition(hOut, { (short)OFFSET_X, (short)(OFFSET_Y + i) });
        std::cout << "##";
        SetConsoleCursorPosition(hOut, { (short)(OFFSET_X + 2 + (boardWidth * 2)), (short)(OFFSET_Y + i) });
        std::cout << "## " << (boardHeight - i);
    }
    SetConsoleCursorPosition(hOut, { (short)OFFSET_X, (short)(OFFSET_Y + boardHeight) });
    for (int i = 0; i < boardWidth + 2; ++i) std::cout << "##";


    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 1) });
    std::cout << "NEXT BLOCK";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 2) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 7) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 10) });
    std::cout << "HOLD BLOCK";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 11) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 16) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 18) });
    std::cout << "LEVEL: " << level;
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 19) });
    std::cout << "----------";
    SetConsoleCursorPosition(hOut, { (short)HUD_X, (short)(OFFSET_Y + 20) });
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

void Board::updateWell(Block::Type nextType, Block::Type holdType, bool hasHold, int level, int score) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (int i = 0; i < boardHeight; ++i) {
        SetConsoleCursorPosition(hOut, { (short)BOARD_DRAW_X, (short)(OFFSET_Y + i) });
        for (int j = 0; j < boardWidth; ++j) {
            if (well[j][i] == 1) std::cout << "()";
            else if (well[j][i] == 2) std::cout << "[]";
            else std::cout << "  ";
        }
    }


    if (firstDraw || nextType != lastNextType) {
        drawBlockPreview(nextType, HUD_X + 1, OFFSET_Y + 3, "()");
        lastNextType = nextType;
    }

    if (hasHold && (firstDraw || !lastHasHold || holdType != lastHoldType)) {
        drawBlockPreview(holdType, HUD_X + 1, OFFSET_Y + 12, "[]");
        lastHoldType = holdType;
        lastHasHold = true;
    }

    if (firstDraw || level != lastLevel) {
        SetConsoleCursorPosition(hOut, { (short)(HUD_X + 7), (short)(OFFSET_Y + 18) });
        std::cout << level << "  "; 
        lastLevel = level;
    }
    
    static int lastScore = -1;
    if (firstDraw || score != lastScore) {
        SetConsoleCursorPosition(hOut, { (short)(HUD_X + 7), (short)(OFFSET_Y + 20) });
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