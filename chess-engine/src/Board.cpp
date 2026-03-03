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

Board::Board() {
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
    //Pawns
    for (int i = 0; i < 8; i++) {
        squares[1][i] = Piece(PieceType::Pawn, PieceColor::White);
        squares[6][i] = Piece(PieceType::Pawn, PieceColor::Black);
    }

    //Rooks
    squares[0][0] = squares[0][7] = Piece(PieceType::Rook, PieceColor::White);
    squares[7][0] = squares[7][7] = Piece(PieceType::Rook, PieceColor::Black);

    //Knights
    squares[0][1] = squares[0][6] = Piece(PieceType::Knight, PieceColor::White);
    squares[7][1] = squares[7][6] = Piece(PieceType::Knight, PieceColor::Black);

    //Bishops
    squares[0][2] = squares[0][5] = Piece(PieceType::Bishop, PieceColor::White);
    squares[7][2] = squares[7][5] = Piece(PieceType::Bishop, PieceColor::Black);

    //Queens
    squares[0][3] = Piece(PieceType::Queen, PieceColor::White);
    squares[7][3] = Piece(PieceType::Queen, PieceColor::Black);

    //Kings
    squares[0][4] = Piece(PieceType::King, PieceColor::White);
    squares[7][4] = Piece(PieceType::King, PieceColor::Black);
}

void Board::print() const {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            cout << static_cast<int>(squares[row][col].type) << " ";
        }
        cout << endl;
    }
}

std::vector<Move> Board::getMovesForPiece(int row, int col) const {
    std::vector<Move> moves;
    
    // Check if position is valid
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return moves;
    }
    
    Piece piece = squares[row][col];
    
    // Check if there is a piece at this position
    if (piece.type == PieceType::None) {
        return moves;
    }
    
    // Generate moves based on piece type
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
        default:
            break;
    }
    
    return moves;
}

std::vector<Move> Board::generateAllMoves(PieceColor side) const {
    std::vector<Move> Allmoves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (squares[row][col].color == side) {
                auto pieceMoves = getMovesForPiece(row, col);
                Allmoves.insert(Allmoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return Allmoves;
}

void Board::makeMove(const Move &move) {

    for (const auto& m : generateAllMoves(squares[move.fromRow][move.fromCol].color)) {
        if (m == move) {
            applyMoveUnchecked(move);
            return;
        }
    }

    cout << "Invalid move: From (" << alphabetToChar(static_cast<alphabet>(move.fromCol)) << move.fromRow+1
         << ") To ("  << alphabetToChar(static_cast<alphabet>(move.toCol)) << move.toRow+1 << ")\n";
    
    
}

void Board::showMoves(const std::vector<Move>& moves) const{
    for (const auto& move : moves) {
        std::cout << "From: (" <<  alphabetToChar(static_cast<alphabet>(move.fromCol)) << move.fromRow+1
        << ") To: ("  << alphabetToChar(static_cast<alphabet>(move.toCol)) << move.toRow+1 << ")\n";
    }
}

Piece Board::getSquare(int row, int col) const {
    return squares[row][col];
}

void Board::setSquare(int row, int col, const Piece& piece) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return;
    }
    squares[row][col] = piece;
}

void Board::clearBoard() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
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

bool Board::isSquareUnderAttack(int row, int col, PieceColor byColor) const {
    int pawnDirection = (byColor == PieceColor::White) ? 1 : -1;
    int pawnRow = row - pawnDirection;
    if (pawnRow >= 0 && pawnRow < 8) {
        if (col - 1 >= 0) {
            Piece leftPawn = squares[pawnRow][col - 1];
            if (leftPawn.type == PieceType::Pawn && leftPawn.color == byColor) {
                return true;
            }
        }
        if (col + 1 < 8) {
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
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
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
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
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
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
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
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
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
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
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
        if (move.toRow == 7 && move.toCol == 0) blackQueenSideRookMoved = true;
        if (move.toRow == 7 && move.toCol == 7) blackKingSideRookMoved = true;
    }

    if (movingPiece.type == PieceType::Pawn &&
        move.toRow == enPassantTargetRow &&
        move.toCol == enPassantTargetCol &&
        move.fromCol != move.toCol &&
        capturedPiece.type == PieceType::None) {
        int capturedPawnRow = (movingPiece.color == PieceColor::White) ? move.toRow - 1 : move.toRow + 1;
        squares[capturedPawnRow][move.toCol] = Piece();
    }

    if (movingPiece.type == PieceType::King && std::abs(move.toCol - move.fromCol) == 2) {
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
        if ((movingPiece.color == PieceColor::White && move.toRow == 7) ||
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
        if (move.fromRow == 7 && move.fromCol == 0) blackQueenSideRookMoved = true;
        if (move.fromRow == 7 && move.fromCol == 7) blackKingSideRookMoved = true;
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
