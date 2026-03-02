//
// Created by ivancc on 3/2/26.
//

#ifndef ASTRACHESS_MOVESETEXPRESSION_H
#define ASTRACHESS_MOVESETEXPRESSION_H
#pragma once

enum class alphabet {
    a, b, c, d, e, f, g, h
};

inline char alphabetToChar(alphabet letter) {
    return static_cast<char>('a' + static_cast<int>(letter));
}

#endif //ASTRACHESS_MOVESETEXPRESSION_H