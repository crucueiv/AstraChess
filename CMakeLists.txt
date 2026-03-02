cmake_minimum_required(VERSION 3.28)
project(AstraChess)

set(CMAKE_CXX_STANDARD 14)

include_directories(chess-engine/src)

add_executable(AstraChess
        chess-engine/src/Board.cpp
        chess-engine/src/Board.h
        chess-engine/src/Engine.cpp
        chess-engine/src/Engine.h
        chess-engine/src/main.cpp
        chess-engine/src/Move.cpp
        chess-engine/src/Move.h
        chess-engine/src/Piece.h)
