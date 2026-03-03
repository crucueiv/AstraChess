//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_BOARD_H
#define ASTRACHESS_BOARD_H
#pragma once
#include "Piece.h"
#include "Tiles.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "Move.h"

struct BoardConfig {
    int rows = 8;
    int cols = 8;
    std::vector<std::vector<TileType>> tileLayout;
};

struct BoardJsonConfig {
    int rows = 8;
    int cols = 8;
    std::vector<std::vector<std::string>> tileLayoutTokens;
};

class Board {
    public:
        Board();
        explicit Board(const BoardConfig& config);
        static BoardConfig fromJsonConfig(const BoardJsonConfig& jsonConfig);

        void initialize();
        void print() const;
        
        std::vector<Move> getMovesForPiece(int row, int col) const;
        std::vector<Move> generateAllMoves(PieceColor side) const;
        Piece getSquare(int row, int col) const;
        void setSquare(int row, int col, const Piece& piece);
        void clearBoard();
        int getRowCount() const;
        int getColCount() const;
        TileType getTileType(int row, int col) const;
        void setTileType(int row, int col, TileType tile);
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
        void registerCustomPieceMoveset(const std::string& customTypeId, std::function<std::vector<Move>(const Board&, int, int)> generator);

    private:
        std::vector<std::vector<Piece>> squares;
        std::vector<std::vector<TileType>> tiles;
        std::unordered_map<std::string, std::function<std::vector<Move>(const Board&, int, int)>> customMoveGenerators;
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
