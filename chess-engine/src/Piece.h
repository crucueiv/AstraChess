//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_PIECE_H
#define ASTRACHESS_PIECE_H
#pragma once
#include <string>
#include <utility>

enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    Custom
};

enum class PieceColor {
    White,
    Black,
    None
};

struct Piece {
    PieceType type;
    PieceColor color;
    std::string customTypeId;
    Piece(PieceType t = PieceType::None, PieceColor c = PieceColor::None, std::string customId = ""):
    type(t), color(c), customTypeId(std::move(customId)) {}
};

#endif //ASTRACHESS_PIECE_H
