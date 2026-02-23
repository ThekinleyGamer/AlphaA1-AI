#ifndef ALPHAA1_H
#define ALPHAA1_H

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

class AlphaA1 {
public:
    std::pair<int, int> getBestMove(const std::vector<std::vector<char>>& board) {
        if (!isBoardShapeValid(board) || isTerminal(board)) {
            return {-1, -1};
        }

        int bestScore = std::numeric_limits<int>::min();
        std::pair<int, int> bestMove{-1, -1};

        for (int row = 0; row < kSize; ++row) {
            for (int col = 0; col < kSize; ++col) {
                if (board[row][col] != kEmpty) {
                    continue;
                }

                auto nextBoard = board;
                nextBoard[row][col] = kAI;

                const int score = minimax(nextBoard, 0, false,
                                          std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {row, col};
                }
            }
        }

        return bestMove;
    }

private:
    static constexpr int kSize = 3;
    static constexpr char kPlayer = 'X';
    static constexpr char kAI = 'O';
    static constexpr char kEmpty = ' ';

    bool isBoardShapeValid(const std::vector<std::vector<char>>& board) const {
        if (static_cast<int>(board.size()) != kSize) {
            return false;
        }
        for (const auto& row : board) {
            if (static_cast<int>(row.size()) != kSize) {
                return false;
            }
        }
        return true;
    }

    bool hasWon(const std::vector<std::vector<char>>& board, char symbol) const {
        for (int i = 0; i < kSize; ++i) {
            if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) {
                return true;
            }
            if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) {
                return true;
            }
        }

        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
            return true;
        }
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
            return true;
        }

        return false;
    }

    bool isDraw(const std::vector<std::vector<char>>& board) const {
        if (hasWon(board, kPlayer) || hasWon(board, kAI)) {
            return false;
        }

        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == kEmpty) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isTerminal(const std::vector<std::vector<char>>& board) const {
        return hasWon(board, kAI) || hasWon(board, kPlayer) || isDraw(board);
    }

    int evaluate(const std::vector<std::vector<char>>& board, int depth) const {
        if (hasWon(board, kAI)) {
            return 10 - depth;
        }
        if (hasWon(board, kPlayer)) {
            return depth - 10;
        }
        return 0;
    }

    int minimax(std::vector<std::vector<char>>& board,
                int depth,
                bool maximizing,
                int alpha,
                int beta) const {
        if (isTerminal(board)) {
            return evaluate(board, depth);
        }

        if (maximizing) {
            int bestScore = std::numeric_limits<int>::min();

            for (int row = 0; row < kSize; ++row) {
                for (int col = 0; col < kSize; ++col) {
                    if (board[row][col] != kEmpty) {
                        continue;
                    }

                    board[row][col] = kAI;
                    const int score = minimax(board, depth + 1, false, alpha, beta);
                    board[row][col] = kEmpty;

                    bestScore = std::max(bestScore, score);
                    alpha = std::max(alpha, bestScore);
                    if (beta <= alpha) {
                        return bestScore;
                    }
                }
            }

            return bestScore;
        }

        int bestScore = std::numeric_limits<int>::max();

        for (int row = 0; row < kSize; ++row) {
            for (int col = 0; col < kSize; ++col) {
                if (board[row][col] != kEmpty) {
                    continue;
                }

                board[row][col] = kPlayer;
                const int score = minimax(board, depth + 1, true, alpha, beta);
                board[row][col] = kEmpty;

                bestScore = std::min(bestScore, score);
                beta = std::min(beta, bestScore);
                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }

        return bestScore;
    }
};

#endif // ALPHAA1_H
