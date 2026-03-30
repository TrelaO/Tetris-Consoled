//Shape.h

#pragma once

#include "Point.h"
#include <vector>

/**
 * @brief Interface for any shape that can be moved and rotated on the board.
 * Provides a common set of operations for different types of tetrominos or game objects.
 */
class Shape {
public:
    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~Shape() = default;

    /** @brief Returns global coordinates of all points composing the shape. */
    virtual std::vector<Point> getPoints() const = 0;

    /** @brief Shifts the shape one unit to the left. */
    virtual void moveLeft() = 0;

    /** @brief Shifts the shape one unit to the right. */
    virtual void moveRight() = 0;

    /** @brief Shifts the shape one unit down. */
    virtual void moveDown() = 0;

    /** @brief Shifts the shape one unit up (usually for collision correction). */
    virtual void moveUp() = 0;

    /** @brief Rotates the shape 90 degrees clockwise. */
    virtual void rotate() = 0;
};