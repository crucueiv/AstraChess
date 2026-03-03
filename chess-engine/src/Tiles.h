#ifndef ASTRACHESS_TILES_H
#define ASTRACHESS_TILES_H
#pragma once

#include <string>

enum class TileType {
    Empty,
    Default
};

inline const char* tileTypeToToken(TileType tileType) {
    switch (tileType) {
        case TileType::Empty:
            return "empty";
        case TileType::Default:
            return "default";
        default:
            return "empty";
    }
}

inline bool tileTypeFromToken(const std::string& token, TileType& outTileType) {
    if (token == "empty") {
        outTileType = TileType::Empty;
        return true;
    }
    if (token == "default") {
        outTileType = TileType::Default;
        return true;
    }
    return false;
}

#endif //ASTRACHESS_TILES_H
