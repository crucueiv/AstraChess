//
// Created by ivancc on 3/2/26.
//

#include "pawnMoveset.h"

std::vector<Move> generatePawnMoves(const Board& board, int row, int col) {
    std::vector<Move> moves;

    Piece piece = board.getSquare(row, col);
    if (piece.type != PieceType::Pawn) {
        return moves;
    }

    int direction = (piece.color == PieceColor::White) ? 1 : -1;
    int nextRow = row + direction;

    // Moving one square forward
    if (nextRow >= 0 && nextRow < 8) {
        if (board.getSquare(nextRow, col).type == PieceType::None) {
            Move move(row, col, nextRow, col);
            if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                moves.push_back(move);
            }
        }
        
        // First move, two squares forward (only if one-square path is clear)
        int startRow = (piece.color == PieceColor::White) ? 1 : 6;
        if (row == startRow) {
            int twoStepRow = row + 2 * direction;
            if (twoStepRow >= 0 && twoStepRow < 8 &&
                board.getSquare(twoStepRow, col).type == PieceType::None) {
                Move twoStep(row, col, twoStepRow, col);
                if (!board.wouldLeaveKingInCheck(twoStep, piece.color)) {
                    moves.push_back(twoStep);
                }
            }
        }
    }

    // Check diagonals for capture
    int captureRow = row + direction;
    if (captureRow >= 0 && captureRow < 8) {
        if (col - 1 >= 0 && board.getSquare(captureRow, col-1).color != piece.color && board.getSquare(captureRow, col-1).color != PieceColor::None) {
            Move move(row, col, captureRow, col-1);
            if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                moves.push_back(move);
            }
        }
        if (col + 1 < 8 && board.getSquare(captureRow, col+1).color != piece.color && board.getSquare(captureRow, col+1).color != PieceColor::None) {
            Move move(row, col, captureRow, col+1);
            if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                moves.push_back(move);
            }
        }
    }
    
    // En passant capture
    int enPassantRow = board.getEnPassantTargetRow();
    int enPassantCol = board.getEnPassantTargetCol();
    if (enPassantRow != -1 && enPassantCol != -1) {
        if (captureRow == enPassantRow) {
            if (col - 1 == enPassantCol || col + 1 == enPassantCol) {
                Move move(row, col, enPassantRow, enPassantCol);
                if (!board.wouldLeaveKingInCheck(move, piece.color)) {
                    moves.push_back(move);
                }
            }
        }
    }
    
    return moves;
}
