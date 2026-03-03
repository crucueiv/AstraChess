//
// Created by ivancc on 4/2/26.
//

#ifndef ASTRACHESS_KNIGHTMOVESET_H
#define ASTRACHESS_KNIGHTMOVESET_H
#pragma once
#include <vector>
#include "Move.h"
#include "Piece.h"
#include <array>
#include "Board.h"
std::vector<Move> generateKnightMoves(const Board& board, int row, int col);

#endif //ASTRACHESS_KNIGHTMOVESET_H