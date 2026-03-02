//
// Created by ivancc on 3/2/26.
//

#include "Board.cpp"

int main() {
    Board board;

    auto moves = board.generateAllMoves(PieceColor::White);

    for (const auto& move : moves) {
        std::cout << "From: (" << move.fromRow << "," << move.fromCol
        << ") To: (" << move.toRow << "," << move.toCol << ")\n";
    }

    board.print();


    return 0;

}