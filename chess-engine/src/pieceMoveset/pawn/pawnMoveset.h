//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_PAWNMOVESET_H
#define ASTRACHESS_PAWNMOVESET_H
#pragma once
#include <vector>
#include "Move.h"
#include "Piece.h"
#include <array>
#include "Board.h"
std::vector<Move> generatePawnMoves(const Board& board, int row, int col);


#endif //ASTRACHESS_PAWNMOVESET_H