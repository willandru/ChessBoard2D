#include "ChessBoard.h"
#include "ChessBoardRenderer.h"
#include "ChessGame.h"
#include "GlfwContext.h"
#include "Shader.h"
#include "Window.h"

#include <glad/glad.h>

#include <filesystem>

namespace
{
constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;
constexpr char kWindowTitle[] = "2D Engine";
constexpr char kVertexShaderPath[] = "basic2d.vert";
constexpr char kFragmentShaderPath[] = "solid.frag";

std::filesystem::path getExecutableDirectory(char* executablePath)
{
    return std::filesystem::absolute(executablePath).parent_path();
}

void renderFrame(
    const ChessBoard& board,
    const ChessGame& game,
    const Shader& shader,
    const ChessBoardRenderer& renderer)
{
    glClearColor(0.12f, 0.13f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    renderer.draw(
        board,
        shader,
        game.currentTurn(),
        game.capturedWhitePieces(),
        game.capturedBlackPieces(),
        game.selectedSquare(),
        game.possibleMoves());
}
}

int main(int, char* argv[])
{
    GlfwContext glfw;
    if (!glfw.isValid())
    {
        return 1;
    }

    Window window(kWindowWidth, kWindowHeight, kWindowTitle);
    if (!window.isValid())
    {
        return 1;
    }

    const std::filesystem::path shaderDirectory = getExecutableDirectory(argv[0]);
    Shader shader(shaderDirectory / kVertexShaderPath, shaderDirectory / kFragmentShaderPath);
    ChessBoard board;
    ChessGame game;
    ChessBoardRenderer renderer;
    if (!shader.isValid() || !renderer.isValid())
    {
        return 1;
    }

    while (!window.shouldClose())
    {
        window.processInput();
        game.update(window, board);
        renderFrame(board, game, shader, renderer);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
