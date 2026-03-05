cmake_minimum_required(VERSION 3.28)
project(AstraChess)

set(CMAKE_CXX_STANDARD 14)

set(CHESS_ENGINE_SOURCES
    chess-engine/src/Board.cpp
    chess-engine/src/Engine.cpp
    chess-engine/src/Multiplayer.cpp
    chess-engine/src/Move.cpp
    chess-engine/src/pieceMoveset/pawn/pawnMoveset.cpp
    chess-engine/src/pieceMoveset/bishop/bishopMoveset.cpp
    chess-engine/src/pieceMoveset/knight/knightMoveset.cpp
    chess-engine/src/pieceMoveset/queen/queenMoveset.cpp
    chess-engine/src/pieceMoveset/king/kingMoveset.cpp
    chess-engine/src/pieceMoveset/rook/rookMoveset.cpp
)

add_library(AstraChessEngine STATIC ${CHESS_ENGINE_SOURCES})
target_include_directories(AstraChessEngine PUBLIC
    ${CMAKE_SOURCE_DIR}/chess-engine/src
)

add_executable(AstraChess chess-engine/src/main.cpp)
target_link_libraries(AstraChess PRIVATE AstraChessEngine)

add_executable(AstraChessMinimalServer
    chess-engine/src/minimalserverc++formultiplayerimplementation/mainserver.cpp
)
target_link_libraries(AstraChessMinimalServer PRIVATE AstraChessEngine)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(AstraChessTests chess-engine/tests/board_rules_test.cpp)
target_link_libraries(AstraChessTests PRIVATE AstraChessEngine GTest::gtest_main)
target_include_directories(AstraChessTests PRIVATE ${CMAKE_SOURCE_DIR}/chess-engine/src)
include(GoogleTest)
gtest_discover_tests(AstraChessTests)
