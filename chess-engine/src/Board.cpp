//
// Created by ivancc on 3/2/26.
//

#include "Board.h"
#include "Piece.h"
#include <iostream>
using namespace std;

Board::Board() {
    initialize();
}

void Board::initialize() {
    //Pawns
    for (int i = 0; i < 8; i++) {
        squares[1][i] = Piece(PieceType::Pawn, PieceColor::White);
        squares[6][i] = Piece(PieceType::Pawn, PieceColor::Black);
    }

    //Rooks
    squares[0][0] = squares[0][7] = Piece(PieceType::Rook, PieceColor::White);
    squares[7][0] = squares[7][7] = Piece(PieceType::Rook, PieceColor::Black);

    //Knights
    squares[0][1] = squares[0][6] = Piece(PieceType::Knight, PieceColor::White);
    squares[7][1] = squares[7][6] = Piece(PieceType::Knight, PieceColor::Black);

    //Bishops
    squares[0][2] = squares[0][5] = Piece(PieceType::Bishop, PieceColor::White);
    squares[7][2] = squares[7][5] = Piece(PieceType::Bishop, PieceColor::Black);

    //Queens
    squares[0][3] = Piece(PieceType::Queen, PieceColor::White);
    squares[7][3] = Piece(PieceType::Queen, PieceColor::Black);

    //Kings
    squares[0][4] = Piece(PieceType::King, PieceColor::White);
    squares[7][4] = Piece(PieceType::King, PieceColor::Black);
}

void Board::print() const {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            cout << static_cast<int>(squares[row][col].type) << " ";
        }
        cout << endl;
    }
}

std::vector<Move> Board::generatePawnMoves(int row, int col) const {
    std::vector<Move> moves;

    Piece piece = squares[row][col];
    if (piece.type != PieceType::Pawn) {
        return moves;
    }

    int direction = (piece.color == PieceColor::White) ? 1 : -1;
    int nextRow = row + direction;

    //Moving forwards
    if (nextRow >= 0 && nextRow < 8) {
        if (squares[nextRow][col].type == PieceType::None) {
            moves.emplace_back(row, col, nextRow, col);
        }
    }
    return moves;
}

std::vector<Move> Board::generateAllMoves(PieceColor side) const {
    std::vector<Move> Allmoves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (squares[row][col].color == side) {
                if (squares[row][col].type == PieceType::Pawn) {
                    auto pawnMoves = generatePawnMoves(row, col);
                    Allmoves.insert(Allmoves.end(), pawnMoves.begin(), pawnMoves.end());
                }
            }
        }
    }
    return Allmoves;
}

void Board::makeMove(const Move &move) {
    squares[move.toRow][move.toCol] = squares[move.fromRow][move.fromRow];
    squares[move.fromRow][move.fromRow]= Piece();
}
