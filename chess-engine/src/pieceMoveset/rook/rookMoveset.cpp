//
// Created by ivancc on 4/2/26.
//

#include "rookMoveset.h"

std::vector<Move> generateRookMoves(const Board& board, int row, int col) {
    std::vector<Move> moves;

    Piece piece = board.getSquare(row, col);
    if (piece.type != PieceType::Rook) {
        return moves;
    }

    // Directions: up, down, left, right
    std::array<std::pair<int, int>, 4> directions = {{
        {1, 0}, {-1, 0}, {0, -1}, {0, 1}
    }};

    for (const auto& dir : directions) {
        int newRow = row + dir.first;
        int newCol = col + dir.second;

        while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            Piece targetPiece = board.getSquare(newRow, newCol);
            if (targetPiece.type == PieceType::None) {
                Move move(row, col, newRow, newCol);
                if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                    moves.push_back(move);
                }
            } else {
                if (targetPiece.color != piece.color) {
                    Move move(row, col, newRow, newCol);
                    if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                        moves.push_back(move);
                    }
                }
                break; // Stop in this direction if we hit a piece
            }
            newRow += dir.first;
            newCol += dir.second;
        }
    }
    return moves;
}
