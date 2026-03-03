//
// Created by ivancc on 3/3/
// 26.
//

#ifndef ASTRACHESS_BISHOPMOVESET_H
#define ASTRACHESS_BISHOPMOVESET_H
#pragma once
#include <vector>
#include "Move.h"
#include "Piece.h"
#include <array>
#include "Board.h"

std::vector<Move> generateBishopMoves(const Board& board, int row, int col);

#endif //ASTRACHESS_BISHOPMOVESET_H
