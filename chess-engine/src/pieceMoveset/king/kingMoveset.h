//
// Created by ivancc on 3/3/26.
//

#ifndef ASTRACHESS_KINGMOVESET_H
#define ASTRACHESS_KINGMOVESET_H
#pragma once
#include <vector>
#include "Move.h"
#include "Piece.h"
#include "Board.h"

std::vector<Move> generateKingMoves(const Board& board, int row, int col);

#endif //ASTRACHESS_KINGMOVESET_H
