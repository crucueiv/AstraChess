#include "Multiplayer.h"

#include <utility>

namespace {
PieceColor oppositeColor(PieceColor color) {
    return color == PieceColor::White ? PieceColor::Black : PieceColor::White;
}

bool containsMove(const std::vector<Move>& moves, const Move& candidate) {
    for (const auto& move : moves) {
        if (move == candidate) {
            return true;
        }
    }
    return false;
}

std::string pieceTypeToken(PieceType type) {
    switch (type) {
        case PieceType::Pawn:
            return "pawn";
        case PieceType::Knight:
            return "knight";
        case PieceType::Bishop:
            return "bishop";
        case PieceType::Rook:
            return "rook";
        case PieceType::Queen:
            return "queen";
        case PieceType::King:
            return "king";
        case PieceType::Custom:
            return "custom";
        case PieceType::None:
            return "none";
    }
    return "none";
}

std::string pieceColorToken(PieceColor color) {
    switch (color) {
        case PieceColor::White:
            return "white";
        case PieceColor::Black:
            return "black";
        case PieceColor::None:
            return "none";
    }
    return "none";
}
}

MultiplayerGame::MultiplayerGame(std::string roomIdValue)
    : board(),
      sideToMove(PieceColor::White),
      roomId(std::move(roomIdValue)),
      seq(0) {}

MultiplayerGameState MultiplayerGame::view() const {
    MultiplayerGameState state;
    state.roomId = roomId;
    state.seq = seq;
    state.turn = colorToSlot(sideToMove);
    state.legalMoves = board.generateAllMoves(sideToMove);
    state.inCheck = board.isKingInCheck(sideToMove);
    state.checkmate = board.isCheckmate(sideToMove);
    state.stalemate = !state.inCheck && state.legalMoves.empty();
    state.finished = state.checkmate || state.stalemate;
    if (state.checkmate) {
        state.winnerPlayerId = slotToPlayerId(colorToSlot(oppositeColor(sideToMove)));
    }

    for (int row = 0; row < board.getRowCount(); row++) {
        for (int col = 0; col < board.getColCount(); col++) {
            Piece piece = board.getSquare(row, col);
            if (piece.type == PieceType::None) {
                continue;
            }

            MultiplayerPieceSnapshot snapshot;
            snapshot.row = row;
            snapshot.col = col;
            snapshot.type = pieceTypeToken(piece.type);
            snapshot.color = pieceColorToken(piece.color);
            snapshot.customTypeId = piece.customTypeId;
            state.pieces.push_back(snapshot);
        }
    }

    return state;
}

MultiplayerApplyMoveResult MultiplayerGame::applyMove(
    MultiplayerPlayerSlot slot,
    const MultiplayerMoveRequest& request
) {
    MultiplayerApplyMoveResult result;

    if (slotToColor(slot) != sideToMove) {
        result.errorCode = "UNAUTHORIZED";
        result.message = "It is not this player's turn.";
        result.state = view();
        return result;
    }

    MultiplayerGameState current = view();
    if (current.finished) {
        result.errorCode = "ROOM_NOT_READY";
        result.message = "Game is already finished.";
        result.state = current;
        return result;
    }

    Move candidate(request.fromRow, request.fromCol, request.toRow, request.toCol);
    if (!containsMove(current.legalMoves, candidate)) {
        result.errorCode = "INVALID_MOVE";
        result.message = "Move is not legal for the current position.";
        result.state = current;
        return result;
    }

    board.makeMove(candidate);
    seq++;
    sideToMove = oppositeColor(sideToMove);

    result.accepted = true;
    result.state = view();
    return result;
}

PieceColor MultiplayerGame::slotToColor(MultiplayerPlayerSlot slot) {
    return slot == MultiplayerPlayerSlot::P1 ? PieceColor::White : PieceColor::Black;
}

MultiplayerPlayerSlot MultiplayerGame::colorToSlot(PieceColor color) {
    return color == PieceColor::White ? MultiplayerPlayerSlot::P1 : MultiplayerPlayerSlot::P2;
}

std::string MultiplayerGame::slotToPlayerId(MultiplayerPlayerSlot slot) {
    return slot == MultiplayerPlayerSlot::P1 ? "p1" : "p2";
}
