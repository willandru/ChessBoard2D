#pragma once

#include "BoardPosition.h"
#include "ChessBoard.h"
#include "Window.h"

#include <optional>
#include <vector>

class ChessGame
{
public:
    void update(Window& window, ChessBoard& board);

    PieceColor currentTurn() const;
    const std::vector<Piece>& capturedWhitePieces() const;
    const std::vector<Piece>& capturedBlackPieces() const;
    const std::optional<BoardPosition>& selectedSquare() const;
    const std::vector<BoardPosition>& possibleMoves() const;

private:
    PieceColor currentTurn_ = PieceColor::White;
    std::vector<Piece> capturedWhitePieces_;
    std::vector<Piece> capturedBlackPieces_;
    std::optional<BoardPosition> selectedSquare_;
    std::vector<BoardPosition> possibleMoves_;
    bool wasMousePressed_ = false;

    void handleBoardClick(ChessBoard& board, BoardPosition clickedSquare);
    void selectPiece(const ChessBoard& board, BoardPosition position);
    void moveSelectedPiece(ChessBoard& board, BoardPosition destination);
    void capturePiece(Piece piece);
    void nextTurn();
    void clearSelection();
};
