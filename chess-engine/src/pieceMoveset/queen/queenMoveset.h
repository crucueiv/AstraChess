//
// Created by ivancc on 3/3/26.
//

#ifndef ASTRACHESS_QUEENMOVESET_H
#define ASTRACHESS_QUEENMOVESET_H
#pragma once
#include <vector>
#include "Move.h"
#include "Piece.h"
#include "Board.h"

std::vector<Move> generateQueenMoves(const Board& board, int row, int col);

#endif //ASTRACHESS_QUEENMOVESET_H
