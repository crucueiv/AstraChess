#ifndef ASTRACHESS_ENGINE_H
#define ASTRACHESS_ENGINE_H

#include "Board.h"
#include "Move.h"

enum class AIDifficulty {
    Easy,
    Medium,
    Hard
};

class Engine {
public:
    static int getDepthForDifficulty(AIDifficulty difficulty);
    static Move findBestMove(const Board& board, PieceColor aiColor, AIDifficulty difficulty);
    static int evaluateBoard(const Board& board, PieceColor perspective);

private:
    static int minimax(
        const Board& board,
        int depth,
        int alpha,
        int beta,
        PieceColor sideToMove,
        PieceColor aiColor
    );
    static PieceColor oppositeColor(PieceColor color);
    static int pieceValue(const Piece& piece);
};

#endif //ASTRACHESS_ENGINE_H
