#include "ChessBoardRenderer.h"

#include "ChessBoardLayout.h"

#include <glad/glad.h>

#include <cmath>
#include <vector>

namespace
{
constexpr float kPi = 3.1415926535f;

struct Color
{
    float r;
    float g;
    float b;
};

float squareLeft(int column)
{
    return ChessBoardLayout::kBoardLeft +
           static_cast<float>(column) * ChessBoardLayout::kSquareSize;
}

float squareTop(int row)
{
    return ChessBoardLayout::kBoardTop -
           static_cast<float>(row) * ChessBoardLayout::kSquareSize;
}

void uploadVertices(unsigned int vbo, const std::vector<float>& vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_DYNAMIC_DRAW);
}

std::vector<float> makeRectangle(
    float left,
    float top,
    float width,
    float height)
{
    const float right = left + width;
    const float bottom = top - height;

    return
    {
        left,  bottom,
        right, bottom,
        right, top,

        left,  bottom,
        right, top,
        left,  top
    };
}

std::vector<float> makeCircle(
    float centerX,
    float centerY,
    float radius,
    int segments)
{
    std::vector<float> vertices;

    vertices.reserve(static_cast<size_t>(segments) * 6);

    for (int i = 0; i < segments; ++i)
    {
        const float angleA =
            2.0f * kPi *
            static_cast<float>(i) /
            static_cast<float>(segments);

        const float angleB =
            2.0f * kPi *
            static_cast<float>(i + 1) /
            static_cast<float>(segments);

        vertices.push_back(centerX);
        vertices.push_back(centerY);

        vertices.push_back(centerX + std::cos(angleA) * radius);
        vertices.push_back(centerY + std::sin(angleA) * radius);

        vertices.push_back(centerX + std::cos(angleB) * radius);
        vertices.push_back(centerY + std::sin(angleB) * radius);
    }

    return vertices;
}

Color squareColor(int row, int column)
{
    if ((row + column) % 2 == 0)
    {
        return {0.88f, 0.82f, 0.68f};
    }

    return {0.42f, 0.55f, 0.34f};
}

Color pieceColor(PieceColor color)
{
    if (color == PieceColor::White)
    {
        return {0.94f, 0.93f, 0.88f};
    }

    return {0.10f, 0.11f, 0.13f};
}

float pieceRadius(PieceType type)
{
    switch (type)
    {
    case PieceType::Pawn:   return 0.055f;
    case PieceType::Knight: return 0.065f;
    case PieceType::Bishop: return 0.065f;
    case PieceType::Rook:   return 0.070f;
    case PieceType::Queen:  return 0.078f;
    case PieceType::King:   return 0.082f;
    case PieceType::None:   return 0.0f;
    }

    return 0.0f;
}
}

ChessBoardRenderer::ChessBoardRenderer()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        nullptr);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ChessBoardRenderer::~ChessBoardRenderer()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

bool ChessBoardRenderer::isValid() const
{
    return vao_ != 0 && vbo_ != 0;
}

void ChessBoardRenderer::draw(
    const ChessBoard& board,
    const Shader& shader,
    PieceColor currentTurn,
    const std::vector<Piece>& capturedWhitePieces,
    const std::vector<Piece>& capturedBlackPieces,
    const std::optional<BoardPosition>& selectedSquare,
    const std::vector<BoardPosition>& possibleMoves) const
{
    glBindVertexArray(vao_);

    for (int row = 0; row < ChessBoard::kSize; ++row)
    {
        for (int column = 0; column < ChessBoard::kSize; ++column)
        {
            drawSquare(row, column, shader);
        }
    }

    if (selectedSquare)
    {
        drawHighlight(*selectedSquare, shader, true);
    }

    for (const BoardPosition& move : possibleMoves)
    {
        drawHighlight(move, shader, false);
    }

    for (int row = 0; row < ChessBoard::kSize; ++row)
    {
        for (int column = 0; column < ChessBoard::kSize; ++column)
        {
            drawPiece(
                row,
                column,
                board.pieceAt(row, column),
                shader);
        }
    }

    drawCapturedPieces(
        capturedWhitePieces,
        -0.95f,
        0.85f,
        shader);

    drawCapturedPieces(
        capturedBlackPieces,
        0.80f,
        0.85f,
        shader);

    drawTurnIndicator(
        currentTurn,
        shader);

    glBindVertexArray(0);
}

void ChessBoardRenderer::drawSquare(
    int row,
    int column,
    const Shader& shader) const
{
    const Color color = squareColor(row, column);

    shader.setColor(
        color.r,
        color.g,
        color.b);

    const std::vector<float> vertices =
        makeRectangle(
            squareLeft(column),
            squareTop(row),
            ChessBoardLayout::kSquareSize,
            ChessBoardLayout::kSquareSize);

    uploadVertices(vbo_, vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ChessBoardRenderer::drawHighlight(
    BoardPosition position,
    const Shader& shader,
    bool isSelected) const
{
    if (isSelected)
    {
        shader.setColor(0.95f, 0.82f, 0.25f);
    }
    else
    {
        shader.setColor(0.20f, 0.65f, 0.95f);
    }

    const float padding =
        isSelected ? 0.018f : 0.045f;

    const std::vector<float> vertices =
        makeRectangle(
            squareLeft(position.column) + padding,
            squareTop(position.row) - padding,
            ChessBoardLayout::kSquareSize - padding * 2.0f,
            ChessBoardLayout::kSquareSize - padding * 2.0f);

    uploadVertices(vbo_, vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ChessBoardRenderer::drawPiece(
    int row,
    int column,
    const Piece& piece,
    const Shader& shader) const
{
    if (piece.type == PieceType::None)
    {
        return;
    }

    const float centerX =
        squareLeft(column) +
        ChessBoardLayout::kSquareSize * 0.5f;

    const float centerY =
        squareTop(row) -
        ChessBoardLayout::kSquareSize * 0.5f;

    const Color color =
        pieceColor(piece.color);

    shader.setColor(
        color.r,
        color.g,
        color.b);

    const std::vector<float> vertices =
        makeCircle(
            centerX,
            centerY,
            pieceRadius(piece.type),
            24);

    uploadVertices(vbo_, vertices);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        static_cast<int>(vertices.size() / 2));
}

void ChessBoardRenderer::drawCapturedPieces(
    const std::vector<Piece>& pieces,
    float left,
    float top,
    const Shader& shader) const
{
    float y = top;

    for (const Piece& piece : pieces)
    {
        const Color color =
            pieceColor(piece.color);

        shader.setColor(
            color.r,
            color.g,
            color.b);

        const std::vector<float> vertices =
            makeCircle(
                left,
                y,
                pieceRadius(piece.type) * 0.8f,
                20);

        uploadVertices(vbo_, vertices);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            static_cast<int>(vertices.size() / 2));

        y -= 0.12f;
    }
}

void ChessBoardRenderer::drawTurnIndicator(
    PieceColor currentTurn,
    const Shader& shader) const
{
    const Color color =
        pieceColor(currentTurn);

    shader.setColor(
        color.r,
        color.g,
        color.b);

    const std::vector<float> vertices =
        makeCircle(
            0.0f,
            0.92f,
            0.05f,
            24);

    uploadVertices(vbo_, vertices);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        static_cast<int>(vertices.size() / 2));
}