//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_BOARD_H
#define ASTRACHESS_BOARD_H
#pragma once
#include "Piece.h"
#include <array>
#include <vector>
#include "Move.h"


class Board {
    public:
        Board();

        void initialize();
        void print() const;
        
        std::vector<Move> getMovesForPiece(int row, int col) const;
        std::vector<Move> generateAllMoves(PieceColor side) const;
        Piece getSquare(int row, int col) const;
        void setSquare(int row, int col, const Piece& piece);
        void clearBoard();
        void makeMove(const Move& move);
        void showMoves(const std::vector<Move>& moves) const;
        bool isSquareUnderAttack(int row, int col, PieceColor byColor) const;
        bool isKingInCheck(PieceColor color) const;
        bool isCheckmate(PieceColor color) const;
        bool wouldLeaveKingInCheck(const Move& move, PieceColor movingSide) const;
        bool canCastleKingSide(PieceColor color) const;
        bool canCastleQueenSide(PieceColor color) const;
        
        // En passant methods
        int getEnPassantTargetRow() const;
        int getEnPassantTargetCol() const;

    private:
        std::array<std::array<Piece, 8>, 8> squares;
        int enPassantTargetRow;
        int enPassantTargetCol;
        bool whiteKingMoved;
        bool blackKingMoved;
        bool whiteQueenSideRookMoved;
        bool whiteKingSideRookMoved;
        bool blackQueenSideRookMoved;
        bool blackKingSideRookMoved;
        void applyMoveUnchecked(const Move& move);
};




#endif //ASTRACHESS_BOARD_H
