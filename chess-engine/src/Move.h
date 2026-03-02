//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_MOVE_H
#define ASTRACHESS_MOVE_H

#pragma once

struct Move {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;

    Move(int fr, int fc, int tr, int tc)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}

    bool operator==(const Move& other) const {
        return fromRow == other.fromRow &&
               fromCol == other.fromCol &&
               toRow == other.toRow &&
               toCol == other.toCol;
    }
};

#endif //ASTRACHESS_MOVE_H