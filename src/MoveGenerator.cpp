#include "MoveGenerator.h"

namespace
{
using Moves = std::vector<BoardPosition>;

bool canLandOn(const ChessBoard& board, BoardPosition position, PieceColor movingColor)
{
    if (!ChessBoard::isInside(position))
    {
        return false;
    }

    const Piece& target = board.pieceAt(position);
    return target.type == PieceType::None || target.color != movingColor;
}

void addStepMove(const ChessBoard& board, Moves& moves, BoardPosition from, int rowDelta, int columnDelta)
{
    const Piece movingPiece = board.pieceAt(from);
    const BoardPosition to{from.row + rowDelta, from.column + columnDelta};
    if (canLandOn(board, to, movingPiece.color))
    {
        moves.push_back(to);
    }
}

void addSlidingMoves(const ChessBoard& board, Moves& moves, BoardPosition from, int rowDelta, int columnDelta)
{
    const Piece movingPiece = board.pieceAt(from);
    BoardPosition to{from.row + rowDelta, from.column + columnDelta};

    while (ChessBoard::isInside(to))
    {
        const Piece& target = board.pieceAt(to);
        if (target.type == PieceType::None)
        {
            moves.push_back(to);
        }
        else
        {
            if (target.color != movingPiece.color)
            {
                moves.push_back(to);
            }

            break;
        }

        to.row += rowDelta;
        to.column += columnDelta;
    }
}

void addKingLikeMoves(const ChessBoard& board, Moves& moves, BoardPosition from)
{
    for (int rowDelta = -1; rowDelta <= 1; ++rowDelta)
    {
        for (int columnDelta = -1; columnDelta <= 1; ++columnDelta)
        {
            if (rowDelta != 0 || columnDelta != 0)
            {
                addStepMove(board, moves, from, rowDelta, columnDelta);
            }
        }
    }
}

void addRookMoves(const ChessBoard& board, Moves& moves, BoardPosition from)
{
    addSlidingMoves(board, moves, from, -1, 0);
    addSlidingMoves(board, moves, from, 1, 0);
    addSlidingMoves(board, moves, from, 0, -1);
    addSlidingMoves(board, moves, from, 0, 1);
}

void addBishopMoves(const ChessBoard& board, Moves& moves, BoardPosition from)
{
    addSlidingMoves(board, moves, from, -1, -1);
    addSlidingMoves(board, moves, from, -1, 1);
    addSlidingMoves(board, moves, from, 1, -1);
    addSlidingMoves(board, moves, from, 1, 1);
}
}

std::vector<BoardPosition> MoveGenerator::possibleMoves(const ChessBoard& board, BoardPosition from)
{
    Moves moves;
    if (!ChessBoard::isInside(from) || board.isEmpty(from))
    {
        return moves;
    }

    switch (board.pieceAt(from).type)
    {
    case PieceType::Pawn:
    case PieceType::King:
        addKingLikeMoves(board, moves, from);
        break;
    case PieceType::Rook:
        addRookMoves(board, moves, from);
        break;
    case PieceType::Knight:
        addStepMove(board, moves, from, -2, -1);
        addStepMove(board, moves, from, -2, 1);
        addStepMove(board, moves, from, -1, -2);
        addStepMove(board, moves, from, -1, 2);
        addStepMove(board, moves, from, 1, -2);
        addStepMove(board, moves, from, 1, 2);
        addStepMove(board, moves, from, 2, -1);
        addStepMove(board, moves, from, 2, 1);
        break;
    case PieceType::Bishop:
        addBishopMoves(board, moves, from);
        break;
    case PieceType::Queen:
        addRookMoves(board, moves, from);
        addBishopMoves(board, moves, from);
        break;
    case PieceType::None:
        break;
    }

    return moves;
}
