#pragma once

#include "BoardPosition.h"
#include "ChessTypes.h"

#include <array>

class ChessBoard
{
public:
    static constexpr int kSize = 8;

    ChessBoard();

    const Piece& pieceAt(int row, int column) const;
    const Piece& pieceAt(BoardPosition position) const;
    void setPiece(int row, int column, Piece piece);
    void setPiece(BoardPosition position, Piece piece);
    void movePiece(BoardPosition from, BoardPosition to);
    bool isEmpty(BoardPosition position) const;
    static bool isInside(BoardPosition position);
    void reset();

private:
    using Row = std::array<Piece, kSize>;
    std::array<Row, kSize> squares_{};
};
