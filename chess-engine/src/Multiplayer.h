#ifndef ASTRACHESS_MULTIPLAYER_H
#define ASTRACHESS_MULTIPLAYER_H

#include "Board.h"

#include <cstdint>
#include <string>
#include <vector>

enum class MultiplayerPlayerSlot {
    P1,
    P2
};

struct MultiplayerMoveRequest {
    std::uint64_t seq = 0;
    int fromRow = -1;
    int fromCol = -1;
    int toRow = -1;
    int toCol = -1;
};

struct MultiplayerPieceSnapshot {
    int row = -1;
    int col = -1;
    std::string type;
    std::string color;
    std::string customTypeId;
};

struct MultiplayerGameState {
    std::string roomId;
    std::uint64_t seq = 0;
    MultiplayerPlayerSlot turn = MultiplayerPlayerSlot::P1;
    bool inCheck = false;
    bool checkmate = false;
    bool stalemate = false;
    bool finished = false;
    std::string winnerPlayerId;
    std::vector<MultiplayerPieceSnapshot> pieces;
    std::vector<Move> legalMoves;
};

struct MultiplayerApplyMoveResult {
    bool accepted = false;
    std::string errorCode;
    std::string message;
    MultiplayerGameState state;
};

class MultiplayerGame {
public:
    explicit MultiplayerGame(std::string roomId = "");

    MultiplayerGameState view() const;
    MultiplayerApplyMoveResult applyMove(MultiplayerPlayerSlot slot, const MultiplayerMoveRequest& request);

    static PieceColor slotToColor(MultiplayerPlayerSlot slot);
    static MultiplayerPlayerSlot colorToSlot(PieceColor color);
    static std::string slotToPlayerId(MultiplayerPlayerSlot slot);

private:
    Board board;
    PieceColor sideToMove;
    std::string roomId;
    std::uint64_t seq;
};

#endif //ASTRACHESS_MULTIPLAYER_H
