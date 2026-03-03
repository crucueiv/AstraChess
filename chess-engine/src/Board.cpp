//
// Created by ivancc on 3/2/26.
//

#include "Board.h"
#include "Piece.h"
#include "movesetExpression.h"
#include "pieceMoveset/pawn/pawnMoveset.h"
#include "pieceMoveset/bishop/bishopMoveset.h"
#include "pieceMoveset/knight/knightMoveset.h"
#include "pieceMoveset/queen/queenMoveset.h"
#include "pieceMoveset/king/kingMoveset.h"
#include "pieceMoveset/rook/rookMoveset.h"
#include <iostream>
#include <cmath>
using namespace std;

namespace {
PieceColor oppositeColor(PieceColor color) {
    return color == PieceColor::White ? PieceColor::Black : PieceColor::White;
}
}

Board::Board() : Board(BoardConfig{}) {}

Board::Board(const BoardConfig& config) {
    int rows = config.rows > 0 ? config.rows : 8;
    int cols = config.cols > 0 ? config.cols : 8;

    squares.assign(rows, std::vector<Piece>(cols, Piece()));
    tiles.assign(rows, std::vector<char>(cols, '.'));
    if (!config.tileLayout.empty() && static_cast<int>(config.tileLayout.size()) == rows) {
        bool valid = true;
        for (int r = 0; r < rows; r++) {
            if (static_cast<int>(config.tileLayout[r].size()) != cols) {
                valid = false;
                break;
            }
        }
        if (valid) {
            tiles = config.tileLayout;
        }
    }

    enPassantTargetRow = -1;
    enPassantTargetCol = -1;
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteQueenSideRookMoved = false;
    whiteKingSideRookMoved = false;
    blackQueenSideRookMoved = false;
    blackKingSideRookMoved = false;

    initialize();
}

void Board::initialize() {
    clearBoard();

    // Standard chess initial placement if board can hold classic setup.
    if (getRowCount() < 8 || getColCount() < 8) {
        return;
    }

    int whiteBackRow = 0;
    int whitePawnRow = 1;
    int blackPawnRow = getRowCount() - 2;
    int blackBackRow = getRowCount() - 1;

    // Pawns
    for (int i = 0; i < 8; i++) {
        squares[whitePawnRow][i] = Piece(PieceType::Pawn, PieceColor::White);
        squares[blackPawnRow][i] = Piece(PieceType::Pawn, PieceColor::Black);
    }

    // Rooks
    squares[whiteBackRow][0] = squares[whiteBackRow][7] = Piece(PieceType::Rook, PieceColor::White);
    squares[blackBackRow][0] = squares[blackBackRow][7] = Piece(PieceType::Rook, PieceColor::Black);

    // Knights
    squares[whiteBackRow][1] = squares[whiteBackRow][6] = Piece(PieceType::Knight, PieceColor::White);
    squares[blackBackRow][1] = squares[blackBackRow][6] = Piece(PieceType::Knight, PieceColor::Black);

    // Bishops
    squares[whiteBackRow][2] = squares[whiteBackRow][5] = Piece(PieceType::Bishop, PieceColor::White);
    squares[blackBackRow][2] = squares[blackBackRow][5] = Piece(PieceType::Bishop, PieceColor::Black);

    // Queens
    squares[whiteBackRow][3] = Piece(PieceType::Queen, PieceColor::White);
    squares[blackBackRow][3] = Piece(PieceType::Queen, PieceColor::Black);

    // Kings
    squares[whiteBackRow][4] = Piece(PieceType::King, PieceColor::White);
    squares[blackBackRow][4] = Piece(PieceType::King, PieceColor::Black);

    whiteKingMoved = false;
    blackKingMoved = false;
    whiteQueenSideRookMoved = false;
    whiteKingSideRookMoved = false;
    blackQueenSideRookMoved = false;
    blackKingSideRookMoved = false;
}

void Board::print() const {
    for (int row = getRowCount() - 1; row >= 0; row--) {
        for (int col = 0; col < getColCount(); col++) {
            cout << static_cast<int>(squares[row][col].type) << " ";
        }
        cout << endl;
    }
}

std::vector<Move> Board::getMovesForPiece(int row, int col) const {
    std::vector<Move> moves;

    if (row < 0 || row >= getRowCount() || col < 0 || col >= getColCount()) {
        return moves;
    }

    Piece piece = squares[row][col];
    if (piece.type == PieceType::None) {
        return moves;
    }

    switch (piece.type) {
        case PieceType::Pawn:
            moves = generatePawnMoves(*this, row, col);
            break;
        case PieceType::Knight:
            moves = generateKnightMoves(*this, row, col);
            break;
        case PieceType::Bishop:
            moves = generateBishopMoves(*this, row, col);
            break;
        case PieceType::Rook:
            moves = generateRookMoves(*this, row, col);
            break;
        case PieceType::Queen:
            moves = generateQueenMoves(*this, row, col);
            break;
        case PieceType::King:
            moves = generateKingMoves(*this, row, col);
            break;
        case PieceType::Custom: {
            auto it = customMoveGenerators.find(piece.customTypeId);
            if (it != customMoveGenerators.end()) {
                moves = it->second(*this, row, col);
            }
            break;
        }
        default:
            break;
    }

    return moves;
}

std::vector<Move> Board::generateAllMoves(PieceColor side) const {
    std::vector<Move> allMoves;
    for (int row = 0; row < getRowCount(); row++) {
        for (int col = 0; col < getColCount(); col++) {
            if (squares[row][col].color == side) {
                auto pieceMoves = getMovesForPiece(row, col);
                allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return allMoves;
}

void Board::makeMove(const Move& move) {
    if (move.fromRow < 0 || move.fromRow >= getRowCount() ||
        move.fromCol < 0 || move.fromCol >= getColCount()) {
        return;
    }

    for (const auto& m : generateAllMoves(squares[move.fromRow][move.fromCol].color)) {
        if (m == move) {
            applyMoveUnchecked(move);
            return;
        }
    }

    if (move.fromCol >= 0 && move.fromCol < 8 && move.toCol >= 0 && move.toCol < 8) {
        cout << "Invalid move: From (" << alphabetToChar(static_cast<alphabet>(move.fromCol)) << move.fromRow + 1
             << ") To (" << alphabetToChar(static_cast<alphabet>(move.toCol)) << move.toRow + 1 << ")\n";
    } else {
        cout << "Invalid move\n";
    }
}

void Board::showMoves(const std::vector<Move>& moves) const {
    for (const auto& move : moves) {
        if (move.fromCol >= 0 && move.fromCol < 8 && move.toCol >= 0 && move.toCol < 8) {
            std::cout << "From: (" << alphabetToChar(static_cast<alphabet>(move.fromCol)) << move.fromRow + 1
                      << ") To: (" << alphabetToChar(static_cast<alphabet>(move.toCol)) << move.toRow + 1 << ")\n";
        } else {
            std::cout << "From: (" << move.fromRow << "," << move.fromCol << ") To: (" << move.toRow << "," << move.toCol << ")\n";
        }
    }
}

Piece Board::getSquare(int row, int col) const {
    if (row < 0 || row >= getRowCount() || col < 0 || col >= getColCount()) {
        return Piece();
    }
    return squares[row][col];
}

void Board::setSquare(int row, int col, const Piece& piece) {
    if (row < 0 || row >= getRowCount() || col < 0 || col >= getColCount()) {
        return;
    }
    squares[row][col] = piece;
}

void Board::clearBoard() {
    for (int row = 0; row < getRowCount(); row++) {
        for (int col = 0; col < getColCount(); col++) {
            squares[row][col] = Piece();
        }
    }
    enPassantTargetRow = -1;
    enPassantTargetCol = -1;
    whiteKingMoved = true;
    blackKingMoved = true;
    whiteQueenSideRookMoved = true;
    whiteKingSideRookMoved = true;
    blackQueenSideRookMoved = true;
    blackKingSideRookMoved = true;
}

int Board::getRowCount() const {
    return static_cast<int>(squares.size());
}

int Board::getColCount() const {
    return getRowCount() > 0 ? static_cast<int>(squares[0].size()) : 0;
}

char Board::getTileType(int row, int col) const {
    if (row < 0 || row >= getRowCount() || col < 0 || col >= getColCount()) {
        return '.';
    }
    return tiles[row][col];
}

void Board::setTileType(int row, int col, char tile) {
    if (row < 0 || row >= getRowCount() || col < 0 || col >= getColCount()) {
        return;
    }
    tiles[row][col] = tile;
}

bool Board::isSquareUnderAttack(int row, int col, PieceColor byColor) const {
    int maxRows = getRowCount();
    int maxCols = getColCount();

    int pawnDirection = (byColor == PieceColor::White) ? 1 : -1;
    int pawnRow = row - pawnDirection;
    if (pawnRow >= 0 && pawnRow < maxRows) {
        if (col - 1 >= 0) {
            Piece leftPawn = squares[pawnRow][col - 1];
            if (leftPawn.type == PieceType::Pawn && leftPawn.color == byColor) {
                return true;
            }
        }
        if (col + 1 < maxCols) {
            Piece rightPawn = squares[pawnRow][col + 1];
            if (rightPawn.type == PieceType::Pawn && rightPawn.color == byColor) {
                return true;
            }
        }
    }

    const int knightOffsets[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    for (const auto& offset : knightOffsets) {
        int r = row + offset[0];
        int c = col + offset[1];
        if (r >= 0 && r < maxRows && c >= 0 && c < maxCols) {
            Piece piece = squares[r][c];
            if (piece.type == PieceType::Knight && piece.color == byColor) {
                return true;
            }
        }
    }

    const int bishopDirections[4][2] = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for (const auto& direction : bishopDirections) {
        int r = row + direction[0];
        int c = col + direction[1];
        while (r >= 0 && r < maxRows && c >= 0 && c < maxCols) {
            Piece piece = squares[r][c];
            if (piece.type != PieceType::None) {
                if (piece.color == byColor &&
                    (piece.type == PieceType::Bishop || piece.type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += direction[0];
            c += direction[1];
        }
    }

    const int rookDirections[4][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    for (const auto& direction : rookDirections) {
        int r = row + direction[0];
        int c = col + direction[1];
        while (r >= 0 && r < maxRows && c >= 0 && c < maxCols) {
            Piece piece = squares[r][c];
            if (piece.type != PieceType::None) {
                if (piece.color == byColor &&
                    (piece.type == PieceType::Rook || piece.type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += direction[0];
            c += direction[1];
        }
    }

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            int r = row + dr;
            int c = col + dc;
            if (r >= 0 && r < maxRows && c >= 0 && c < maxCols) {
                Piece piece = squares[r][c];
                if (piece.type == PieceType::King && piece.color == byColor) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::isKingInCheck(PieceColor color) const {
    for (int row = 0; row < getRowCount(); row++) {
        for (int col = 0; col < getColCount(); col++) {
            Piece piece = squares[row][col];
            if (piece.type == PieceType::King && piece.color == color) {
                return isSquareUnderAttack(row, col, oppositeColor(color));
            }
        }
    }
    return false;
}

bool Board::isCheckmate(PieceColor color) const {
    if (!isKingInCheck(color)) {
        return false;
    }
    auto moves = generateAllMoves(color);
    return moves.empty();
}

bool Board::wouldLeaveKingInCheck(const Move& move, PieceColor movingSide) const {
    Board copy = *this;
    copy.applyMoveUnchecked(move);
    return copy.isKingInCheck(movingSide);
}

bool Board::canCastleKingSide(PieceColor color) const {
    if (getRowCount() != 8 || getColCount() != 8) {
        return false;
    }

    int row = (color == PieceColor::White) ? 0 : 7;
    Piece king = squares[row][4];
    Piece rook = squares[row][7];
    bool kingMoved = (color == PieceColor::White) ? whiteKingMoved : blackKingMoved;
    bool rookMoved = (color == PieceColor::White) ? whiteKingSideRookMoved : blackKingSideRookMoved;

    if (kingMoved || rookMoved ||
        king.type != PieceType::King || king.color != color ||
        rook.type != PieceType::Rook || rook.color != color) {
        return false;
    }

    if (squares[row][5].type != PieceType::None || squares[row][6].type != PieceType::None) {
        return false;
    }

    PieceColor enemyColor = oppositeColor(color);
    if (isKingInCheck(color) ||
        isSquareUnderAttack(row, 5, enemyColor) ||
        isSquareUnderAttack(row, 6, enemyColor)) {
        return false;
    }

    return true;
}

bool Board::canCastleQueenSide(PieceColor color) const {
    if (getRowCount() != 8 || getColCount() != 8) {
        return false;
    }

    int row = (color == PieceColor::White) ? 0 : 7;
    Piece king = squares[row][4];
    Piece rook = squares[row][0];
    bool kingMoved = (color == PieceColor::White) ? whiteKingMoved : blackKingMoved;
    bool rookMoved = (color == PieceColor::White) ? whiteQueenSideRookMoved : blackQueenSideRookMoved;

    if (kingMoved || rookMoved ||
        king.type != PieceType::King || king.color != color ||
        rook.type != PieceType::Rook || rook.color != color) {
        return false;
    }

    if (squares[row][1].type != PieceType::None ||
        squares[row][2].type != PieceType::None ||
        squares[row][3].type != PieceType::None) {
        return false;
    }

    PieceColor enemyColor = oppositeColor(color);
    if (isKingInCheck(color) ||
        isSquareUnderAttack(row, 3, enemyColor) ||
        isSquareUnderAttack(row, 2, enemyColor)) {
        return false;
    }

    return true;
}

void Board::applyMoveUnchecked(const Move& move) {
    Piece movingPiece = squares[move.fromRow][move.fromCol];
    Piece capturedPiece = squares[move.toRow][move.toCol];

    if (capturedPiece.type == PieceType::Rook) {
        if (move.toRow == 0 && move.toCol == 0) whiteQueenSideRookMoved = true;
        if (move.toRow == 0 && move.toCol == 7) whiteKingSideRookMoved = true;
        if (move.toRow == getRowCount() - 1 && move.toCol == 0) blackQueenSideRookMoved = true;
        if (move.toRow == getRowCount() - 1 && move.toCol == 7) blackKingSideRookMoved = true;
    }

    if (movingPiece.type == PieceType::Pawn &&
        move.toRow == enPassantTargetRow &&
        move.toCol == enPassantTargetCol &&
        move.fromCol != move.toCol &&
        capturedPiece.type == PieceType::None) {
        int capturedPawnRow = (movingPiece.color == PieceColor::White) ? move.toRow - 1 : move.toRow + 1;
        squares[capturedPawnRow][move.toCol] = Piece();
    }

    if (movingPiece.type == PieceType::King && std::abs(move.toCol - move.fromCol) == 2 && getColCount() == 8) {
        if (move.toCol == 6) {
            squares[move.toRow][5] = squares[move.toRow][7];
            squares[move.toRow][7] = Piece();
        } else if (move.toCol == 2) {
            squares[move.toRow][3] = squares[move.toRow][0];
            squares[move.toRow][0] = Piece();
        }
    }

    squares[move.toRow][move.toCol] = movingPiece;
    squares[move.fromRow][move.fromCol] = Piece();

    if (movingPiece.type == PieceType::Pawn) {
        if ((movingPiece.color == PieceColor::White && move.toRow == getRowCount() - 1) ||
            (movingPiece.color == PieceColor::Black && move.toRow == 0)) {
            squares[move.toRow][move.toCol] = Piece(PieceType::Queen, movingPiece.color);
        }
    }

    if (movingPiece.type == PieceType::King) {
        if (movingPiece.color == PieceColor::White) {
            whiteKingMoved = true;
        } else {
            blackKingMoved = true;
        }
    }

    if (movingPiece.type == PieceType::Rook) {
        if (move.fromRow == 0 && move.fromCol == 0) whiteQueenSideRookMoved = true;
        if (move.fromRow == 0 && move.fromCol == 7) whiteKingSideRookMoved = true;
        if (move.fromRow == getRowCount() - 1 && move.fromCol == 0) blackQueenSideRookMoved = true;
        if (move.fromRow == getRowCount() - 1 && move.fromCol == 7) blackKingSideRookMoved = true;
    }

    if (movingPiece.type == PieceType::Pawn && std::abs(move.toRow - move.fromRow) == 2) {
        enPassantTargetRow = (move.fromRow + move.toRow) / 2;
        enPassantTargetCol = move.fromCol;
    } else {
        enPassantTargetRow = -1;
        enPassantTargetCol = -1;
    }
}

int Board::getEnPassantTargetRow() const {
    return enPassantTargetRow;
}

int Board::getEnPassantTargetCol() const {
    return enPassantTargetCol;
}

void Board::registerCustomPieceMoveset(const std::string& customTypeId, std::function<std::vector<Move>(const Board&, int, int)> generator) {
    customMoveGenerators[customTypeId] = std::move(generator);
}
