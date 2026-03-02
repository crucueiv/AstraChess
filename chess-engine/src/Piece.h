//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_PIECE_H
#define ASTRACHESS_PIECE_H
#pragma once

enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class PieceColor {
    White,
    Black,
    None
};

struct Piece {
    PieceType type;
    PieceColor color;
    Piece(PieceType t = PieceType::None, PieceColor c = PieceColor::None):
    type(t), color(c) {}
};

#endif //ASTRACHESS_PIECE_H