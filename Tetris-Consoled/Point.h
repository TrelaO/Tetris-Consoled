//Point.h

#pragma once

/**
 * @brief Represents a 2D coordinate point with basic movement operations.
 * Used as a base building block for shapes and local/global positions.
 */
class Point {
public:
    int x; /**< Horizontal coordinate (column). */
    int y; /**< Vertical coordinate (row). */

    /** @brief Initializes point with given coordinates. Default is 0,0. */
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    /** @brief Decrements the X coordinate. */
    void moveLeft() { x--; }

    /** @brief Increments the X coordinate. */
    void moveRight() { x++; }

    /** @brief Increments the Y coordinate. */
    void moveDown() { y++; }

    /** @brief Decrements the Y coordinate. */
    void moveUp() { y--; }
};