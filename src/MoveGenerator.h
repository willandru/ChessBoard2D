#pragma once

#include "BoardPosition.h"
#include "ChessBoard.h"

#include <vector>

class MoveGenerator
{
public:
    static std::vector<BoardPosition> possibleMoves(const ChessBoard& board, BoardPosition from);
};
