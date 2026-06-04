#include "ChessGame.h"

#include "ChessBoardLayout.h"
#include "MoveGenerator.h"

#include <GLFW/glfw3.h>

#include <algorithm>

namespace
{
std::optional<BoardPosition> screenToBoard(Window& window)
{
    double cursorX = 0.0;
    double cursorY = 0.0;
    int width = 0;
    int height = 0;

    window.cursorPosition(cursorX, cursorY);
    window.framebufferSize(width, height);

    if (width <= 0 || height <= 0)
    {
        return std::nullopt;
    }

    const float normalizedX = static_cast<float>(cursorX) / static_cast<float>(width) * 2.0f - 1.0f;
    const float normalizedY = 1.0f - static_cast<float>(cursorY) / static_cast<float>(height) * 2.0f;

    if (normalizedX < ChessBoardLayout::kBoardLeft ||
        normalizedX >= ChessBoardLayout::kBoardLeft + ChessBoardLayout::kBoardSize ||
        normalizedY > ChessBoardLayout::kBoardTop ||
        normalizedY <= ChessBoardLayout::kBoardTop - ChessBoardLayout::kBoardSize)
    {
        return std::nullopt;
    }

    const int column = static_cast<int>((normalizedX - ChessBoardLayout::kBoardLeft) / ChessBoardLayout::kSquareSize);
    const int row = static_cast<int>((ChessBoardLayout::kBoardTop - normalizedY) / ChessBoardLayout::kSquareSize);
    return BoardPosition{row, column};
}

bool containsPosition(const std::vector<BoardPosition>& positions, BoardPosition position)
{
    return std::find(positions.begin(), positions.end(), position) != positions.end();
}
}

void ChessGame::update(Window& window, ChessBoard& board)
{
    const bool isMousePressed = window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    if (isMousePressed && !wasMousePressed_)
    {
        const std::optional<BoardPosition> clickedSquare = screenToBoard(window);
        if (clickedSquare)
        {
            handleBoardClick(board, *clickedSquare);
        }
        else
        {
            clearSelection();
        }
    }

    wasMousePressed_ = isMousePressed;
}

const std::optional<BoardPosition>& ChessGame::selectedSquare() const
{
    return selectedSquare_;
}

const std::vector<BoardPosition>& ChessGame::possibleMoves() const
{
    return possibleMoves_;
}

PieceColor ChessGame::currentTurn() const
{
    return currentTurn_;
}

const std::vector<Piece>& ChessGame::capturedWhitePieces() const
{
    return capturedWhitePieces_;
}

const std::vector<Piece>& ChessGame::capturedBlackPieces() const
{
    return capturedBlackPieces_;
}

void ChessGame::handleBoardClick(ChessBoard& board, BoardPosition clickedSquare)
{
    if (selectedSquare_ && containsPosition(possibleMoves_, clickedSquare))
    {
        moveSelectedPiece(board, clickedSquare);
        return;
    }

    if (!board.isEmpty(clickedSquare) && board.pieceAt(clickedSquare).color == currentTurn_)
    {
        selectPiece(board, clickedSquare);
        return;
    }

    clearSelection();
}

void ChessGame::selectPiece(const ChessBoard& board, BoardPosition position)
{
    selectedSquare_ = position;
    possibleMoves_ = MoveGenerator::possibleMoves(board, position);
}

void ChessGame::moveSelectedPiece(ChessBoard& board, BoardPosition destination)
{
    const Piece capturedPiece = board.pieceAt(destination);
    if (capturedPiece.type != PieceType::None)
    {
        capturePiece(capturedPiece);
    }

    board.movePiece(*selectedSquare_, destination);
    clearSelection();
    nextTurn();
}

void ChessGame::capturePiece(Piece piece)
{
    if (piece.color == PieceColor::White)
    {
        capturedWhitePieces_.push_back(piece);
        return;
    }

    capturedBlackPieces_.push_back(piece);
}

void ChessGame::nextTurn()
{
    currentTurn_ = currentTurn_ == PieceColor::White ? PieceColor::Black : PieceColor::White;
}

void ChessGame::clearSelection()
{
    selectedSquare_.reset();
    possibleMoves_.clear();
}
