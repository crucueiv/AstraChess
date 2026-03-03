//
// Created by ivancc on 4/2/26.
//

#include "knightMoveset.h"

std::vector<Move> generateKnightMoves(const Board& board, int row, int col) {
    std::vector<Move> moves;

    Piece piece = board.getSquare(row, col);
    if (piece.type != PieceType::Knight) {
        return moves;
    }

    std::array<std::pair<int, int>, 8> knightMoves = {{
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    }};

    for (const auto& move : knightMoves) {
        int newRow = row + move.first;
        int newCol = col + move.second;

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            Piece targetPiece = board.getSquare(newRow, newCol);
            if (targetPiece.color != piece.color) {
                Move move(row, col, newRow, newCol);
                if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                    moves.push_back(move);
                }
            }
        }
    }
    return moves;
}
