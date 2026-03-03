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

    board.makeMove(Move(0, 1, 2, 0)); // Move white pawn from a2 to a4
    
    cout << "\n";

    board.print();

    

    return 0;

}