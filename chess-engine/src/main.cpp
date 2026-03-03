#include "Board.h"
#include "Engine.h"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace {
PieceColor oppositeColor(PieceColor color) {
    if (color == PieceColor::White) {
        return PieceColor::Black;
    }
    return PieceColor::White;
}

bool hasMove(const std::vector<Move>& moves, const Move& candidate) {
    for (const auto& move : moves) {
        if (move == candidate) {
            return true;
        }
    }
    return false;
}

bool parseMoveText(std::string text, Move& outMove) {
    text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char c) {
        return std::isspace(c);
    }), text.end());

    if (text.size() != 4) {
        return false;
    }

    char fromFile = static_cast<char>(std::tolower(text[0]));
    char fromRank = text[1];
    char toFile = static_cast<char>(std::tolower(text[2]));
    char toRank = text[3];

    if (fromFile < 'a' || fromFile > 'h' || toFile < 'a' || toFile > 'h') {
        return false;
    }
    if (fromRank < '1' || fromRank > '8' || toRank < '1' || toRank > '8') {
        return false;
    }

    outMove = Move(fromRank - '1', fromFile - 'a', toRank - '1', toFile - 'a');
    return true;
}
}

int main() {
    Board board;
    PieceColor sideToMove = PieceColor::White;

    int mode = 1;
    std::cout << "Select mode:\n1) Human vs AI\n2) Human vs Human\n> ";
    std::cin >> mode;

    bool vsAi = mode != 2;
    PieceColor aiColor = PieceColor::Black;
    AIDifficulty difficulty = AIDifficulty::Medium;

    if (vsAi) {
        int difficultyChoice = 2;
        std::cout << "Select AI difficulty:\n1) Easy\n2) Medium\n3) Hard\n> ";
        std::cin >> difficultyChoice;
        if (difficultyChoice == 1) {
            difficulty = AIDifficulty::Easy;
        } else if (difficultyChoice == 3) {
            difficulty = AIDifficulty::Hard;
        }
    }

    while (true) {
        std::cout << "\nCurrent board:\n";
        board.print();

        auto legalMoves = board.generateAllMoves(sideToMove);
        if (legalMoves.empty()) {
            if (board.isKingInCheck(sideToMove)) {
                std::cout << "Checkmate. Winner: "
                          << (oppositeColor(sideToMove) == PieceColor::White ? "White" : "Black")
                          << "\n";
            } else {
                std::cout << "Stalemate.\n";
            }
            break;
        }

        if (vsAi && sideToMove == aiColor) {
            Move aiMove = Engine::findBestMove(board, aiColor, difficulty);
            if (aiMove.fromRow < 0) {
                std::cout << "AI has no legal moves.\n";
                break;
            }
            std::cout << "AI move: "
                      << static_cast<char>('a' + aiMove.fromCol) << aiMove.fromRow + 1
                      << static_cast<char>('a' + aiMove.toCol) << aiMove.toRow + 1
                      << "\n";
            board.makeMove(aiMove);
        } else {
            std::string input;
            std::cout << (sideToMove == PieceColor::White ? "White" : "Black")
                      << " move (e2e4 or 'quit'): ";
            std::cin >> input;

            if (input == "quit") {
                break;
            }

            Move move(-1, -1, -1, -1);
            if (!parseMoveText(input, move) || !hasMove(legalMoves, move)) {
                std::cout << "Invalid move format or illegal move.\n";
                continue;
            }
            board.makeMove(move);
        }

        sideToMove = oppositeColor(sideToMove);
    }

    return 0;
}
