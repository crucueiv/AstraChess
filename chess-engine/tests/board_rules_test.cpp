#include <gtest/gtest.h>
#include "Board.h"

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
