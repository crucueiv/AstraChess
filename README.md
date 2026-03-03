# AstraChess

AstraChess is a C++ chess engine focused on move generation, legality checks, and a clean path for future engine features.

## Current status

- Piece movesets implemented: Pawn, Knight, Bishop, Rook, Queen, King
- Special rules implemented: castling, en passant, promotion (to Queen)
- Rule checks implemented: check and checkmate detection helpers
- Test coverage: GoogleTest suite for core move/rule behavior

## Project structure

```text
chess-engine/src/
  Board.h / Board.cpp                 # board state + rule application
  Piece.h                             # piece definitions
  Move.h                              # move model
  pieceMoveset/<piece>/...            # per-piece move generation
chess-engine/tests/
  board_rules_test.cpp                # rule and moveset tests
```

## Build

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

## Run tests

```bash
ctest --test-dir cmake-build-debug --output-on-failure
```

## Extending the engine (today)

- Add/update piece move logic in `chess-engine/src/pieceMoveset/`
- Wire moveset usage through `Board::getMovesForPiece`
- Add tests in `chess-engine/tests/board_rules_test.cpp`

## Modular extension API (W.I.P)

### 1) Create a custom board size/layout

```cpp
BoardConfig config;
config.rows = 10;
config.cols = 10;
config.tileLayout = std::vector<std::vector<char>>(10, std::vector<char>(10, 'G'));
Board board(config);
```

### 2) Register a custom piece moveset

```cpp
board.registerCustomPieceMoveset(\"Camel\", [](const Board& board, int row, int col) {
    std::vector<Move> moves;
    // add your custom movement logic here
    return moves;
});

board.setSquare(4, 4, Piece(PieceType::Custom, PieceColor::White, \"Camel\"));
auto moves = board.getMovesForPiece(4, 4);
```

Classic chess remains the default by calling `Board board;`.

## Contributor guide

When editing rules, keep this order:
1. Add or change board logic
2. Add or update a test that proves behavior
3. Build and run tests before committing

## Roadmap

- Improve modularity to support custom pieces without hard-coded dispatch
- Support configurable board sizes/tiles beyond classic 8x8
- Continue improving separation of rules, state, and move generation
