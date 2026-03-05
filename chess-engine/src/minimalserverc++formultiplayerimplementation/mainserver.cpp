//
// Created by ivancc on 3/5/26
//

#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include "../Multiplayer.h"
#include "multiplayernecesarylibs/httplib.h"      // https://github.com/yhirose/cpp-httplib (single header)
#include "multiplayernecesarylibs/json.hpp"       // https://github.com/nlohmann/json (single header)

using json = nlohmann::json;

namespace {
std::unordered_map<std::string, MultiplayerGame> roomStore;
std::mutex roomStoreMutex;

json makeError(const std::string& reason, const std::string& message = "") {
    return {
            {"code", reason},
            {"message", message}
    };
}

void setError(httplib::Response& res, int status, const std::string& code, const std::string& message, bool includeAccepted = false) {
    json err = makeError(code, message);
    if (includeAccepted) {
        err["accepted"] = false;
        if (!err.contains("nextState")) {
            err["nextState"] = nullptr;
        }
        if (!err.contains("legalMoves")) {
            err["legalMoves"] = json::array();
        }
        if (!err.contains("finished")) {
            err["finished"] = false;
        }
    }
    res.status = status;
    res.set_content(err.dump(), "application/json");
}

json moveToJson(const Move& move) {
    return json{
            {"fromRow", move.fromRow},
            {"fromCol", move.fromCol},
            {"toRow", move.toRow},
            {"toCol", move.toCol}
    };
}

std::string slotToContractPlayerId(MultiplayerPlayerSlot slot) {
    return slot == MultiplayerPlayerSlot::P1 ? "P1" : "P2";
}

json playerIdToContract(const std::string& playerId) {
    if (playerId == "p1" || playerId == "P1") {
        return "P1";
    }
    if (playerId == "p2" || playerId == "P2") {
        return "P2";
    }
    return playerId.empty() ? nullptr : json(playerId);
}

json stateToJson(const MultiplayerGameState& state) {
    json pieces = json::array();
    for (const auto& piece : state.pieces) {
        pieces.push_back(json{
                {"row", piece.row},
                {"col", piece.col},
                {"type", piece.type},
                {"color", piece.color},
                {"customTypeId", piece.customTypeId}
        });
    }

    json legalMoves = json::array();
    for (const auto& move : state.legalMoves) {
        legalMoves.push_back(moveToJson(move));
    }

    json out{
            {"roomId", state.roomId},
            {"seq", state.seq},
            {"turn", slotToContractPlayerId(state.turn)},
            {"inCheck", state.inCheck},
            {"checkmate", state.checkmate},
            {"stalemate", state.stalemate},
            {"finished", state.finished},
            {"pieces", pieces},
            {"legalMoves", legalMoves}
    };
    out["winnerPlayerId"] = playerIdToContract(state.winnerPlayerId);
    return out;
}

bool parsePlayerSlot(const std::string& playerId, MultiplayerPlayerSlot& slot) {
    if (playerId == "p1" || playerId == "P1") {
        slot = MultiplayerPlayerSlot::P1;
        return true;
    }
    if (playerId == "p2" || playerId == "P2") {
        slot = MultiplayerPlayerSlot::P2;
        return true;
    }
    return false;
}

bool parseMoveRequest(const json& body, MultiplayerMoveRequest& moveRequest, std::string& errorMessage) {
    if (!body.contains("seq") || !body.contains("move")) {
        errorMessage = "seq and move are required";
        return false;
    }
    const auto& move = body.at("move");
    if (!move.is_object() || !move.contains("fromRow") || !move.contains("fromCol") || !move.contains("toRow") || !move.contains("toCol")) {
        errorMessage = "move must include fromRow, fromCol, toRow, and toCol";
        return false;
    }
    if (!body.at("seq").is_number_unsigned() ||
        !move.at("fromRow").is_number_integer() ||
        !move.at("fromCol").is_number_integer() ||
        !move.at("toRow").is_number_integer() ||
        !move.at("toCol").is_number_integer()) {
        errorMessage = "seq must be unsigned and move coordinates must be integers";
        return false;
    }

    moveRequest.seq = body.at("seq").get<std::uint64_t>();
    moveRequest.fromRow = move.at("fromRow").get<int>();
    moveRequest.fromCol = move.at("fromCol").get<int>();
    moveRequest.toRow = move.at("toRow").get<int>();
    moveRequest.toCol = move.at("toCol").get<int>();
    return true;
}
}

int main() {
    httplib::Server svr;

    svr.Post("/games/create", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            if (!body.contains("roomId")) {
                setError(res, 400, "BAD_REQUEST", "roomId is required");
                return;
            }
            if (!body.at("roomId").is_string()) {
                setError(res, 400, "BAD_REQUEST", "roomId must be a string");
                return;
            }

            const std::string roomId = body.at("roomId").get<std::string>();
            if (roomId.empty()) {
                setError(res, 400, "BAD_REQUEST", "roomId must be non-empty");
                return;
            }

            std::lock_guard<std::mutex> lock(roomStoreMutex);
            if (roomStore.find(roomId) != roomStore.end()) {
                setError(res, 409, "ROOM_ALREADY_EXISTS", "roomId already exists");
                return;
            }

            auto inserted = roomStore.emplace(roomId, MultiplayerGame(roomId));
            json out{
                    {"roomId", roomId},
                    {"state", stateToJson(inserted.first->second.view())}
            };
            res.status = 201;
            res.set_content(out.dump(), "application/json");
        } catch (const json::exception& e) {
            setError(res, 400, "BAD_REQUEST", e.what());
        } catch (const std::exception& e) {
            setError(res, 500, "INTERNAL_ERROR", e.what());
        }
    });

    svr.Get("/games/state", [](const httplib::Request& req, httplib::Response& res) {
        try {
            if (!req.has_param("roomId")) {
                setError(res, 400, "BAD_REQUEST", "roomId is required");
                return;
            }

            const std::string roomId = req.get_param_value("roomId");
            std::lock_guard<std::mutex> lock(roomStoreMutex);
            auto it = roomStore.find(roomId);
            if (it == roomStore.end()) {
                setError(res, 404, "ROOM_NOT_FOUND", "roomId was not found");
                return;
            }

            json out{
                    {"roomId", roomId},
                    {"state", stateToJson(it->second.view())}
            };
            res.status = 200;
            res.set_content(out.dump(), "application/json");
        } catch (const std::exception& e) {
            setError(res, 500, "INTERNAL_ERROR", e.what());
        }
    });

    svr.Post("/moves/validate", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);

            if (!body.contains("roomId") || !body.contains("playerId")) {
                setError(res, 400, "BAD_REQUEST", "roomId and playerId are required", true);
                return;
            }
            if (!body.at("roomId").is_string() || !body.at("playerId").is_string()) {
                setError(res, 400, "BAD_REQUEST", "roomId and playerId must be strings", true);
                return;
            }

            MultiplayerPlayerSlot slot;
            if (!parsePlayerSlot(body.at("playerId").get<std::string>(), slot)) {
                setError(res, 401, "UNAUTHORIZED", "playerId is not mapped to an allowed player", true);
                return;
            }

            MultiplayerMoveRequest moveRequest;
            std::string parseMoveError;
            if (!parseMoveRequest(body, moveRequest, parseMoveError)) {
                setError(res, 400, "BAD_REQUEST", parseMoveError, true);
                return;
            }

            const std::string roomId = body.at("roomId").get<std::string>();
            std::lock_guard<std::mutex> lock(roomStoreMutex);
            auto it = roomStore.find(roomId);
            if (it == roomStore.end()) {
                setError(res, 404, "ROOM_NOT_FOUND", "roomId was not found", true);
                return;
            }

            const auto currentState = it->second.view();
            if (moveRequest.seq != currentState.seq) {
                setError(res, 422, "INVALID_MOVE", "seq does not match current state", true);
                return;
            }

            auto result = it->second.applyMove(slot, moveRequest);
            if (!result.accepted) {
                json rejectedStateJson = stateToJson(result.state);
                const std::string code = result.errorCode.empty() ? "INVALID_MOVE" : result.errorCode;
                int status = code == "UNAUTHORIZED" ? 401 : 422;
                res.status = status;
                res.set_content(json{
                        {"accepted", false},
                        {"nextState", rejectedStateJson},
                        {"legalMoves", rejectedStateJson.at("legalMoves")},
                        {"finished", result.state.finished},
                        {"code", code},
                        {"message", result.message}
                }.dump(), "application/json");
                return;
            }

            const auto& nextState = result.state;
            json nextStateJson = stateToJson(nextState);
            res.status = 200;
            res.set_content(json{
                    {"accepted", true},
                    {"nextState", nextStateJson},
                    {"legalMoves", nextStateJson.at("legalMoves")},
                    {"finished", nextState.finished},
                    {"code", nullptr}
            }.dump(), "application/json");
        } catch (const json::exception& e) {
            setError(res, 400, "BAD_REQUEST", e.what(), true);
        } catch (const std::exception& e) {
            setError(res, 500, "INTERNAL_ERROR", e.what(), true);
        }
    });

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
        res.set_content(R"({"ok":true,"service":"astrachess-engine-bridge"})", "application/json");
    });

    const char* host = "0.0.0.0";
    int port = 8081;
    std::cout << "AstraChess bridge listening on http://" << host << ":" << port << std::endl;
    svr.listen(host, port);
}
