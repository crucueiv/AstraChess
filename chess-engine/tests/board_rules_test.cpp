#include <gtest/gtest.h>
#include "Board.h"
#include "Engine.h"

namespace {
bool hasMove(const std::vector<Move>& moves, int fr, int fc, int tr, int tc) {
    for (const auto& m : moves) {
        if (m.fromRow == fr && m.fromCol == fc && m.toRow == tr && m.toCol == tc) {
            return true;
        }
    }
    return false;
}
}

TEST(MoveSets, PawnMovesetFromStart) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(1, 3, Piece(PieceType::Pawn, PieceColor::White));

    auto moves = board.getMovesForPiece(1, 3);
    EXPECT_TRUE(hasMove(moves, 1, 3, 2, 3));
    EXPECT_TRUE(hasMove(moves, 1, 3, 3, 3));
}

TEST(MoveSets, KnightMoveset) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Knight, PieceColor::White));

    auto moves = board.getMovesForPiece(3, 3);
    EXPECT_EQ(moves.size(), 8);
}

TEST(MoveSets, BishopMoveset) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Bishop, PieceColor::White));

    auto moves = board.getMovesForPiece(3, 3);
    EXPECT_EQ(moves.size(), 13);
}

TEST(MoveSets, RookMoveset) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Rook, PieceColor::White));

    auto moves = board.getMovesForPiece(3, 3);
    EXPECT_EQ(moves.size(), 14);
}

TEST(MoveSets, QueenMoveset) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Queen, PieceColor::White));

    auto moves = board.getMovesForPiece(3, 3);
    EXPECT_EQ(moves.size(), 27);
}

TEST(MoveSets, KingMoveset) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));

    auto moves = board.getMovesForPiece(0, 4);
    EXPECT_EQ(moves.size(), 5);
}

TEST(Captures, PawnAndKnightCaptures) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Pawn, PieceColor::White));
    board.setSquare(4, 4, Piece(PieceType::Knight, PieceColor::Black));
    board.setSquare(2, 2, Piece(PieceType::Knight, PieceColor::White));
    board.setSquare(4, 3, Piece(PieceType::Bishop, PieceColor::Black));

    auto pawnMoves = board.getMovesForPiece(3, 3);
    auto knightMoves = board.getMovesForPiece(2, 2);
    EXPECT_TRUE(hasMove(pawnMoves, 3, 3, 4, 4));
    EXPECT_TRUE(hasMove(knightMoves, 2, 2, 4, 3));
}

TEST(Castling, KingSideCastlingWorks) {
    Board board;
    board.makeMove(Move(1, 4, 2, 4));
    board.makeMove(Move(6, 0, 5, 0));
    board.makeMove(Move(0, 6, 2, 5));
    board.makeMove(Move(5, 0, 4, 0));
    board.makeMove(Move(0, 5, 1, 4));

    auto kingMoves = board.getMovesForPiece(0, 4);
    EXPECT_TRUE(hasMove(kingMoves, 0, 4, 0, 6));

    board.makeMove(Move(0, 4, 0, 6));
    EXPECT_EQ(board.getSquare(0, 6).type, PieceType::King);
    EXPECT_EQ(board.getSquare(0, 5).type, PieceType::Rook);
}

TEST(CheckAndMate, DetectCheckAndCheckmate) {
    Board checkBoard;
    checkBoard.clearBoard();
    checkBoard.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    checkBoard.setSquare(7, 0, Piece(PieceType::King, PieceColor::Black));
    checkBoard.setSquare(7, 4, Piece(PieceType::Rook, PieceColor::Black));
    EXPECT_TRUE(checkBoard.isKingInCheck(PieceColor::White));

    Board mateBoard;
    mateBoard.clearBoard();
    mateBoard.setSquare(0, 7, Piece(PieceType::King, PieceColor::White));
    mateBoard.setSquare(2, 5, Piece(PieceType::King, PieceColor::Black));
    mateBoard.setSquare(1, 6, Piece(PieceType::Queen, PieceColor::Black));
    EXPECT_TRUE(mateBoard.isCheckmate(PieceColor::White));
}

TEST(Promotion, PawnPromotesToQueen) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(6, 0, Piece(PieceType::Pawn, PieceColor::White));

    board.makeMove(Move(6, 0, 7, 0));
    Piece promoted = board.getSquare(7, 0);
    EXPECT_EQ(promoted.type, PieceType::Queen);
    EXPECT_EQ(promoted.color, PieceColor::White);
}

TEST(Modularity, SupportsCustomBoardDimensionsAndTiles) {
    BoardConfig config;
    config.rows = 10;
    config.cols = 10;
    config.tileLayout = std::vector<std::vector<TileType>>(10, std::vector<TileType>(10, TileType::Default));
    config.tileLayout[0][0] = TileType::Empty;

    Board board(config);
    board.clearBoard();
    EXPECT_EQ(board.getRowCount(), 10);
    EXPECT_EQ(board.getColCount(), 10);
    EXPECT_EQ(board.getTileType(0, 0), TileType::Empty);
    EXPECT_EQ(board.getTileType(5, 5), TileType::Default);
}

TEST(Modularity, SupportsCustomPieceMoveGeneratorRegistration) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(4, 4, Piece(PieceType::Custom, PieceColor::White, "Camel", 350));

    board.registerCustomPieceMoveset("Camel", [](const Board& b, int row, int col) {
        std::vector<Move> moves;
        int targetRow = row + 3;
        int targetCol = col + 1;
        if (targetRow < b.getRowCount() && targetCol < b.getColCount()) {
            Piece target = b.getSquare(targetRow, targetCol);
            Piece source = b.getSquare(row, col);
            if (target.color != source.color) {
                Move move(row, col, targetRow, targetCol);
                if (!b.wouldLeaveKingInCheck(move, source.color)) {
                    moves.push_back(move);
                }
            }
        }
        return moves;
    });

    auto moves = board.getMovesForPiece(4, 4);
    EXPECT_TRUE(hasMove(moves, 4, 4, 7, 5));
}

TEST(AI, DifficultyDepthMappingIncreasesWithHardness) {
    EXPECT_LT(Engine::getDepthForDifficulty(AIDifficulty::Easy),
              Engine::getDepthForDifficulty(AIDifficulty::Medium));
    EXPECT_LT(Engine::getDepthForDifficulty(AIDifficulty::Medium),
              Engine::getDepthForDifficulty(AIDifficulty::Hard));
}

TEST(AI, PrefersCapturingHigherValuePiece) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Queen, PieceColor::White));
    board.setSquare(3, 6, Piece(PieceType::Rook, PieceColor::Black));

    Move best = Engine::findBestMove(board, PieceColor::White, AIDifficulty::Hard);
    EXPECT_EQ(best.fromRow, 3);
    EXPECT_EQ(best.fromCol, 3);
    EXPECT_EQ(best.toRow, 3);
    EXPECT_EQ(best.toCol, 6);
}

TEST(CustomPiece, RejectsValueOutsideAllowedRange) {
    EXPECT_THROW(Piece(PieceType::Custom, PieceColor::White, "BadLow", 100), std::invalid_argument);
    EXPECT_THROW(Piece(PieceType::Custom, PieceColor::White, "BadHigh", 900), std::invalid_argument);
}

TEST(CustomPiece, EvaluationUsesPerPieceCustomValue) {
    Board board;
    board.clearBoard();
    board.setSquare(0, 4, Piece(PieceType::King, PieceColor::White));
    board.setSquare(7, 4, Piece(PieceType::King, PieceColor::Black));
    board.setSquare(3, 3, Piece(PieceType::Custom, PieceColor::White, "Camel", 700));
    board.setSquare(4, 4, Piece(PieceType::Custom, PieceColor::Black, "Falcon", 200));

    EXPECT_EQ(Engine::evaluateBoard(board, PieceColor::White), 500);
    EXPECT_EQ(Engine::evaluateBoard(board, PieceColor::Black), -500);
}
