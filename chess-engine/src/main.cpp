//
// Created by ivancc on 3/2/26.
//

#include "Board.h"
#include <iostream>
using namespace std;

int main() {
    Board board;

    auto moves = board.generateAllMoves(PieceColor::White);

    board.showMoves(moves);

    board.print();

    board.makeMove(Move(1, 0, 3, 0)); // Move white pawn from a2 to a4

    cout << "\n";

    board.print();

    moves = board.generateAllMoves(PieceColor::Black);

    board.showMoves(moves);

    board.makeMove(Move(6, 1, 4, 1));

    board.print();

    moves = board.generateAllMoves(PieceColor::White);

    board.showMoves(moves);

    return 0;

}