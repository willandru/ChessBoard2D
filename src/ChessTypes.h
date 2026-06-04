#pragma once

enum class PieceColor
{
    White,
    Black
};

enum class PieceType
{
    None,
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

struct Piece
{
    PieceType type = PieceType::None;
    PieceColor color = PieceColor::White;
};
