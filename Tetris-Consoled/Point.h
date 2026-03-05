#pragma once

/**
 * @brief Represents a 2D coordinate point with basic movement operations.
 */
class Point {
public:
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    void moveLeft() { x--; }
    void moveRight() { x++; }
    void moveDown() { y++; }
    void moveUp() { y--; }
};