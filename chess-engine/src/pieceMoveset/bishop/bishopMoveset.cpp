//
// Created by ivancc on 3/3/26.
//

#include "bishopMoveset.h"

std::vector<Move> generateBishopMoves(const Board& board, int row, int col) {
    std::vector<Move> moves;

    Piece piece = board.getSquare(row, col);
    if (piece.type != PieceType::Bishop) {
        return moves;
    }

    const int directions[4][2] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1}
    };

    for (const auto& direction : directions) {
        int currentRow = row + direction[0];
        int currentCol = col + direction[1];

        while (currentRow >= 0 && currentRow < 8 && currentCol >= 0 && currentCol < 8) {
            Piece target = board.getSquare(currentRow, currentCol);

            if (target.type == PieceType::None) {
                Move move(row, col, currentRow, currentCol);
                if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                    moves.push_back(move);
                }
            } else {
                if (target.color != piece.color) {
                    Move move(row, col, currentRow, currentCol);
                    if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                        moves.push_back(move);
                    }
                }
                break;
            }

            currentRow += direction[0];
            currentCol += direction[1];
        }
    }

    return moves;
}
