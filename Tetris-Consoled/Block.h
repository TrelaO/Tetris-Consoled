//Nlock.h

#pragma once

#include "Point.h"
#include <vector>

/**
 * @brief Represents a specific Tetromino block composed of four points.
 * Inherits from Point to store its center/reference position.
 */
class Block : public Point {
public:
    /** @brief Supported Tetromino types. */
    enum class Type { I, J, L, O, S, T, Z };

    /** @brief Constructs a block of specific type at the given start coordinates. */
    Block(Type type, int startX, int startY);

    /** @brief Moves all constituent points one unit left. */
    void moveLeft();

    /** @brief Moves all constituent points one unit right. */
    void moveRight();

    /** @brief Moves all constituent points one unit down. */
    void moveDown();

    /** @brief Moves all constituent points one unit up. */
    void moveUp();

    /** @brief Performs 90-degree clockwise rotation around the block's pivot point. */
    void rotate();

    /** @brief Accessor for the list of points that make up the block's current shape. */
    const std::vector<Point>& getPoints() const;

    /** @brief Returns the type of the current block. */
    Type getType() const { return type; }

private:
    Type type;                 /**< Internal tetromino type. */
    std::vector<Point> points; /**< Collection of points in global board space. */

    /** @brief Sets up initial point offsets based on the tetromino type. */
    void initializePoints(int startX, int startY);
};