#include "Engine.h"

#include <algorithm>
#include <limits>

int Engine::getDepthForDifficulty(AIDifficulty difficulty) {
    switch (difficulty) {
        case AIDifficulty::Easy:
            return 2;
        case AIDifficulty::Medium:
            return 3;
        case AIDifficulty::Hard:
            return 4;
    }
    return 2;
}

Move Engine::findBestMove(const Board& board, PieceColor aiColor, AIDifficulty difficulty) {
    auto moves = board.generateAllMoves(aiColor);
    if (moves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    int depth = getDepthForDifficulty(difficulty);
    int bestScore = std::numeric_limits<int>::min();
    Move bestMove = moves.front();

    for (const auto& move : moves) {
        Board copy = board;
        copy.makeMove(move);

        int score = minimax(
            copy,
            depth - 1,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
            oppositeColor(aiColor),
            aiColor
        );

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

int Engine::evaluateBoard(const Board& board, PieceColor perspective) {
    int score = 0;

    for (int row = 0; row < board.getRowCount(); row++) {
        for (int col = 0; col < board.getColCount(); col++) {
            Piece piece = board.getSquare(row, col);
            if (piece.type == PieceType::None) {
                continue;
            }
            int value = pieceValue(piece);
            if (piece.color == perspective) {
                score += value;
            } else if (piece.color != PieceColor::None) {
                score -= value;
            }
        }
    }

    if (board.isCheckmate(perspective)) {
        score -= 100000;
    }
    if (board.isCheckmate(oppositeColor(perspective))) {
        score += 100000;
    }

    return score;
}

int Engine::minimax(
    const Board& board,
    int depth,
    int alpha,
    int beta,
    PieceColor sideToMove,
    PieceColor aiColor
) {
    auto legalMoves = board.generateAllMoves(sideToMove);
    if (depth == 0 || legalMoves.empty()) {
        if (legalMoves.empty() && board.isKingInCheck(sideToMove)) {
            return sideToMove == aiColor ? -100000 - depth : 100000 + depth;
        }
        return evaluateBoard(board, aiColor);
    }

    if (sideToMove == aiColor) {
        int best = std::numeric_limits<int>::min();
        for (const auto& move : legalMoves) {
            Board copy = board;
            copy.makeMove(move);
            int score = minimax(copy, depth - 1, alpha, beta, oppositeColor(sideToMove), aiColor);
            best = std::max(best, score);
            alpha = std::max(alpha, best);
            if (beta <= alpha) {
                break;
            }
        }
        return best;
    }

    int best = std::numeric_limits<int>::max();
    for (const auto& move : legalMoves) {
        Board copy = board;
        copy.makeMove(move);
        int score = minimax(copy, depth - 1, alpha, beta, oppositeColor(sideToMove), aiColor);
        best = std::min(best, score);
        beta = std::min(beta, best);
        if (beta <= alpha) {
            break;
        }
    }
    return best;
}

PieceColor Engine::oppositeColor(PieceColor color) {
    if (color == PieceColor::White) {
        return PieceColor::Black;
    }
    if (color == PieceColor::Black) {
        return PieceColor::White;
    }
    return PieceColor::None;
}

int Engine::pieceValue(const Piece& piece) {
    if (piece.type == PieceType::Custom) {
        return piece.customValue;
    }

    switch (piece.type) {
        case PieceType::Pawn:
            return 100;
        case PieceType::Knight:
            return 320;
        case PieceType::Bishop:
            return 330;
        case PieceType::Rook:
            return 500;
        case PieceType::Queen:
            return 900;
        case PieceType::King:
            return 20000;
        case PieceType::None:
            return 0;
    }
    return 0;
}
