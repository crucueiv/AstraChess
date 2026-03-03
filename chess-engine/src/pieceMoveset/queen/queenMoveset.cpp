//
// Created by ivancc on 3/3/26.
//

#include "queenMoveset.h"

std::vector<Move> generateQueenMoves(const Board& board, int row, int col) {
    std::vector<Move> moves;
    Piece piece = board.getSquare(row, col);

    if (piece.type != PieceType::Queen) {
        return moves;
    }

    const int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const auto& direction : directions) {
        int r = row + direction[0];
        int c = col + direction[1];

        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Piece targetPiece = board.getSquare(r, c);
            if (targetPiece.color == piece.color) {
                break;
            }

            Move move(row, col, r, c);
            if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                moves.push_back(move);
            }

            if (targetPiece.type != PieceType::None) {
                break;
            }

            r += direction[0];
            c += direction[1];
        }
    }

    return moves;
}
