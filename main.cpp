#include "AlphaA1.h"

#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr int kBoardSize = 3;
constexpr char kPlayer = 'X';
constexpr char kAI = 'O';
constexpr char kEmpty = ' ';

using Board = std::vector<std::vector<char>>;

Board createBoard() {
    return Board(kBoardSize, std::vector<char>(kBoardSize, kEmpty));
}

bool isWin(const Board& board, char symbol) {
    for (int i = 0; i < kBoardSize; ++i) {
        if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) ||
            (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)) {
            return true;
        }
    }

    return (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
           (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol);
}

bool isDraw(const Board& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == kEmpty) {
                return false;
            }
        }
    }
    return !isWin(board, kPlayer) && !isWin(board, kAI);
}

void printBoard(const Board& board) {
    std::cout << "\n   1   2   3\n";
    for (int row = 0; row < kBoardSize; ++row) {
        std::cout << row + 1 << "  " << board[row][0] << " | " << board[row][1] << " | "
                  << board[row][2] << '\n';
        if (row < kBoardSize - 1) {
            std::cout << "  ---+---+---\n";
        }
    }
    std::cout << '\n';
}

void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool getPlayerMove(const Board& board, std::pair<int, int>& move, bool& quitRequested) {
    int row = 0;
    int col = 0;

    std::cout << "Enter your move (row and column, 1-3 1-3): ";
    if (!(std::cin >> row >> col)) {
        if (std::cin.eof()) {
            quitRequested = true;
            return false;
        }
        clearInputStream();
        std::cout << "Invalid input. Please enter two numbers.\n";
        return false;
    }

    if (row < 1 || row > 3 || col < 1 || col > 3) {
        std::cout << "Out of range. Row and column must be between 1 and 3.\n";
        return false;
    }

    row -= 1;
    col -= 1;

    if (board[row][col] != kEmpty) {
        std::cout << "Cell is already occupied. Choose another move.\n";
        return false;
    }

    move = {row, col};
    return true;
}

bool playerStarts() {
    while (true) {
        std::cout << "Do you want to play first? (y/n): ";
        std::string response;
        if (!(std::cin >> response)) {
            clearInputStream();
            continue;
        }

        if (!response.empty()) {
            const char c = static_cast<char>(std::tolower(response[0]));
            if (c == 'y') {
                return true;
            }
            if (c == 'n') {
                return false;
            }
        }

        std::cout << "Please answer with 'y' or 'n'.\n";
    }
}

void playGame() {
    AlphaA1 ai;
    Board board = createBoard();
    bool playerTurn = playerStarts();

    std::cout << "\nYou are 'X'. AlphaA1 is 'O'.\n";

    while (true) {
        printBoard(board);

        if (playerTurn) {
            std::pair<int, int> playerMove{-1, -1};
            bool quitRequested = false;
            while (!getPlayerMove(board, playerMove, quitRequested)) {
                if (quitRequested) {
                    std::cout << "Input stream closed. Exiting game.\n";
                    return;
                }
            }
            board[playerMove.first][playerMove.second] = kPlayer;
            std::cout << "You played: (" << (playerMove.first + 1) << ", " << (playerMove.second + 1)
                      << ")\n";
        } else {
            const std::pair<int, int> aiMove = ai.getBestMove(board);
            if (aiMove.first == -1 || aiMove.second == -1) {
                std::cout << "AlphaA1 found no legal move.\n";
                break;
            }
            board[aiMove.first][aiMove.second] = kAI;
            std::cout << "AlphaA1 played: (" << (aiMove.first + 1) << ", " << (aiMove.second + 1)
                      << ")\n";
        }

        if (isWin(board, kPlayer)) {
            printBoard(board);
            std::cout << "You win.\n";
            break;
        }

        if (isWin(board, kAI)) {
            printBoard(board);
            std::cout << "AlphaA1 wins.\n";
            break;
        }

        if (isDraw(board)) {
            printBoard(board);
            std::cout << "It's a draw.\n";
            break;
        }

        playerTurn = !playerTurn;
    }
}

}  // namespace

int main() {
    std::cout << "=== Tic-Tac-Toe vs AlphaA1 ===\n";
    playGame();
    return 0;
}
