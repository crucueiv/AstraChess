//
// Created by ivancc on 4/2/26.
//

#ifndef ASTRACHESS_ROOKMOVESET_H
#define ASTRACHESS_ROOKMOVESET_H
#pragma once
#include <vector>
#include "Move.h"
#include "Piece.h"
#include <array>
#include "Board.h"
std::vector<Move> generateRookMoves(const Board& board, int row, int col);


#endif //ASTRACHESS_ROOKMOVESET_H