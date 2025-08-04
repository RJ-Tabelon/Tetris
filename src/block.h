#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block {
    public:
        Block();
        void Draw(int offsetX, int offsetY);
        void Move(int rows, int cols);
        std::vector<Position> GetCellPositions();
        void Rotate();
        void UndoRotation();
        int id; // to distinguish different blocks
        std::map<int, std::vector<Position>> cells; // represent cells that the block occupies in a 4x4 grid for all rotation states

    private:
        int cellSize;
        int rotationState;
        std::vector<Color> colors;
        int rowOffset;
        int colOffset;
};  