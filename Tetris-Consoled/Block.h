//Block.h

#pragma once

#include "Point.h"
#include <vector>

class Block : public Point {
public:
    enum class Type { I, J, L, O, S, T, Z };

    Block(Type type, int startX, int startY);

    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    void rotate();

    const std::vector<Point>& getPoints() const;
    Type getType() const { return type; } 

private:
    Type type;
    std::vector<Point> points;

    void initializePoints(int startX, int startY);
};