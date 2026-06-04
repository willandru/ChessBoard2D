#pragma once

#include "ChessBoard.h"
#include "Shader.h"

#include <optional>
#include <vector>

class ChessBoardRenderer
{
public:
    ChessBoardRenderer();
    ~ChessBoardRenderer();

    ChessBoardRenderer(const ChessBoardRenderer&) = delete;
    ChessBoardRenderer& operator=(const ChessBoardRenderer&) = delete;

    bool isValid() const;
    void draw(
        const ChessBoard& board,
        const Shader& shader,
        PieceColor currentTurn,
        const std::vector<Piece>& capturedWhitePieces,
        const std::vector<Piece>& capturedBlackPieces,
        const std::optional<BoardPosition>& selectedSquare,
        const std::vector<BoardPosition>& possibleMoves) const;

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;

    void drawSquare(int row, int column, const Shader& shader) const;
    void drawHighlight(BoardPosition position, const Shader& shader, bool isSelected) const;
    void drawPiece(int row, int column, const Piece& piece, const Shader& shader) const;
    void drawCapturedPieces(const std::vector<Piece>& pieces, float left, float top, const Shader& shader) const;
    void drawTurnIndicator(PieceColor currentTurn, const Shader& shader) const;
};
