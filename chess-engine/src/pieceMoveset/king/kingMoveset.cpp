//
// Created by ivancc on 3/3/26.
//

#include "kingMoveset.h"

std::vector<Move> generateKingMoves(const Board& board, int row, int col) {
    std::vector<Move> moves;
    Piece piece = board.getSquare(row, col);

    if (piece.type != PieceType::King) {
        return moves;
    }

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }

            int targetRow = row + dr;
            int targetCol = col + dc;
            if (targetRow < 0 || targetRow >= 8 || targetCol < 0 || targetCol >= 8) {
                continue;
            }

            Piece targetPiece = board.getSquare(targetRow, targetCol);
            if (targetPiece.color == piece.color) {
                continue;
            }

            Move move(row, col, targetRow, targetCol);
            if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                moves.push_back(move);
            }
        }
    }

    if (board.canCastleKingSide(piece.color)) {
        Move kingSideCastle(row, col, row, col + 2);
        if (!board.wouldLeaveKingInCheck(kingSideCastle, piece.color)) {
            moves.push_back(kingSideCastle);
        }
    }

    if (board.canCastleQueenSide(piece.color)) {
        Move queenSideCastle(row, col, row, col - 2);
        if (!board.wouldLeaveKingInCheck(queenSideCastle, piece.color)) {
            moves.push_back(queenSideCastle);
        }
    }

    return moves;
}
