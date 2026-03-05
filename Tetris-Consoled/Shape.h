#pragma once
#include "Point.h"
#include <vector>

/**
 * @brief Interface for any shape that can be moved and rotated on the board.
 */
class Shape {
public:
    virtual ~Shape() = default;
    virtual std::vector<Point> getPoints() const = 0;
    virtual void moveLeft() = 0;
    virtual void moveRight() = 0;
    virtual void moveDown() = 0;
    virtual void moveUp() = 0;
    virtual void rotate() = 0;
};