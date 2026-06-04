#include "ChessBoard.h"

namespace
{
constexpr Piece kEmpty{};

constexpr std::array<PieceType, ChessBoard::kSize> kBackRank = {
    PieceType::Rook,
    PieceType::Knight,
    PieceType::Bishop,
    PieceType::Queen,
    PieceType::King,
    PieceType::Bishop,
    PieceType::Knight,
    PieceType::Rook,
};
}

ChessBoard::ChessBoard()
{
    reset();
}

const Piece& ChessBoard::pieceAt(int row, int column) const
{
    return squares_[row][column];
}

const Piece& ChessBoard::pieceAt(BoardPosition position) const
{
    return pieceAt(position.row, position.column);
}

void ChessBoard::setPiece(int row, int column, Piece piece)
{
    squares_[row][column] = piece;
}

void ChessBoard::setPiece(BoardPosition position, Piece piece)
{
    setPiece(position.row, position.column, piece);
}

void ChessBoard::movePiece(BoardPosition from, BoardPosition to)
{
    setPiece(to, pieceAt(from));
    setPiece(from, kEmpty);
}

bool ChessBoard::isEmpty(BoardPosition position) const
{
    return pieceAt(position).type == PieceType::None;
}

bool ChessBoard::isInside(BoardPosition position)
{
    return position.row >= 0 &&
        position.row < kSize &&
        position.column >= 0 &&
        position.column < kSize;
}

void ChessBoard::reset()
{
    for (Row& row : squares_)
    {
        row.fill(kEmpty);
    }

    for (int column = 0; column < kSize; ++column)
    {
        squares_[0][column] = {kBackRank[column], PieceColor::Black};
        squares_[1][column] = {PieceType::Pawn, PieceColor::Black};
        squares_[6][column] = {PieceType::Pawn, PieceColor::White};
        squares_[7][column] = {kBackRank[column], PieceColor::White};
    }
}
