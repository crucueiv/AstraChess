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

    //Moving forwards
    if (nextRow >= 0 && nextRow < 8) {
        if (board.getSquare(nextRow, col).type == PieceType::None) {
            moves.emplace_back(row, col, nextRow, col);
        }
    }

    //First move, two forwards
    if (nextRow >= 0 && nextRow < 8) {
        if (board.getSquare(row, col).color == PieceColor::White) {
            if (row == 1) {
                direction = 2;
            }
        }
        else if (row == 6){
            direction = -2;
        }

    }
    nextRow = row + direction;

    if (nextRow >= 0 && nextRow < 8) {
        if (board.getSquare(nextRow, col).type == PieceType::None) {
            moves.emplace_back(row, col, nextRow, col);
        }
    }

    //Check diagonals for kill
    direction = (piece.color == PieceColor::White) ? 1 : -1;
    int captureRow = row + direction;
    if (captureRow >= 0 && captureRow < 8) {
        if (col - 1 >= 0 && board.getSquare(captureRow, col-1).color != piece.color && board.getSquare(captureRow, col-1).color != PieceColor::None) {
            moves.emplace_back(row, col, captureRow, col-1);
        }
        if (col + 1 < 8 && board.getSquare(captureRow, col+1).color != piece.color && board.getSquare(captureRow, col+1).color != PieceColor::None) {
            moves.emplace_back(row, col, captureRow, col+1);
        }
    }
    return moves;
}