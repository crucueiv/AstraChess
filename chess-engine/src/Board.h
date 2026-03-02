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
        std::vector<Move> generatePawnMoves(int row, int col) const;
        std::vector<Move> generateAllMoves(PieceColor side) const;
    private:
        std::array<std::array<Piece, 8>, 8> squares;
};




#endif //ASTRACHESS_BOARD_H