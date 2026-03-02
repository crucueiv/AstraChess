//
// Created by ivancc on 3/2/26.
//

#include "Board.h"
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
    squares[0][0] = squares[0][7] = Piece(PieceType::Rook, Color::White);
    squares[7][0] = squares[7][7] = Piece(PieceType::Rook, Color::Black);

    //Knights
    squares[0][1] = squares[0][6] = Piece(PieceType::Knight, Color::White);
    squares[7][1] = squares[7][6] = Piece(PieceType::Knight, Color::Black);

    //Bishops
    squares[0][2] = squares[0][5] = Piece(PieceType::Bishop, Color::White);
    squares[7][2] = squares[7][5] = Piece(PieceType::Bishop, Color::Black);

    //Queens
    squares[0][3] = Piece(PieceType::Queen, Color::White);
    squares[7][3] = Piece(PieceType::Queen, Color::Black);

    //Kings
    squares[0][4] = Piece(PieceType::King, Color::White);
    squares[7][4] = Piece(PieceType::King, Color::Black);
}

void Board::print() const {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            cout << static_cast<int>(squares[row][col].type); << " ";
        }
        cout << endl;
    }
}