cmake_minimum_required(VERSION 3.28)
project(AstraChess)

set(CMAKE_CXX_STANDARD 14)

add_executable(AstraChess
    chess-engine/src/Board.cpp
    chess-engine/src/Engine.cpp
    chess-engine/src/main.cpp
    chess-engine/src/Move.cpp
    chess-engine/src/pieceMoveset/pawn/pawnMoveset.cpp
    chess-engine/src/pieceMoveset/bishop/bishopMoveset.cpp
    chess-engine/src/pieceMoveset/knight/knightMoveset.cpp
    chess-engine/src/pieceMoveset/queen/queenMoveset.cpp
)

target_include_directories(AstraChess PRIVATE
    ${CMAKE_SOURCE_DIR}/chess-engine/src
)
