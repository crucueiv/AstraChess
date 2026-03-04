//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_PIECE_H
#define ASTRACHESS_PIECE_H
#pragma once
#include <string>
#include <stdexcept>
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
    static constexpr int kMinCustomPieceValue = 101;
    static constexpr int kMaxCustomPieceValue = 899;

    PieceType type;
    PieceColor color;
    std::string customTypeId;
    int customValue;

    Piece(
        PieceType t = PieceType::None,
        PieceColor c = PieceColor::None,
        std::string customId = "",
        int value = 0
    ) : type(t), color(c), customTypeId(std::move(customId)), customValue(value) {
        if (type == PieceType::Custom) {
            if (customValue < kMinCustomPieceValue || customValue > kMaxCustomPieceValue) {
                throw std::invalid_argument("Custom piece value must be in range [101, 899].");
            }
        } else {
            customValue = 0;
        }
    }
};

#endif //ASTRACHESS_PIECE_H
