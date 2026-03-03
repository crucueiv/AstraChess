//
// Created by ivancc on 3/2/26.
//

#include "Board.h"
#include "Piece.h"
#include "movesetExpression.h"
#include "pieceMoveset/pawn/pawnMoveset.h"
#include "pieceMoveset/knight/knightMoveset.h"
#include <iostream>
using namespace std;

Board::Board() {
    enPassantTargetRow = -1;
    enPassantTargetCol = -1;
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
            //moves = generateBishopMoves(*this, row, col);
            break;
        case PieceType::Rook:
            //moves = generateRookMoves(*this, row, col);
            break;
        case PieceType::Queen:
            //moves = generateQueenMoves(*this, row, col);
            break;
        case PieceType::King:
            //moves = generateKingMoves(*this, row, col);
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
            Piece movingPiece = squares[move.fromRow][move.fromCol];
            
            // Check if this is an en passant capture
            if (movingPiece.type == PieceType::Pawn && 
                move.toRow == enPassantTargetRow && 
                move.toCol == enPassantTargetCol) {
                // Remove the captured pawn
                int capturedPawnRow = (movingPiece.color == PieceColor::White) ? move.toRow - 1 : move.toRow + 1;
                squares[capturedPawnRow][move.toCol] = Piece();
            }
            
            // Make the move
            squares[move.toRow][move.toCol] = movingPiece;
            squares[move.fromRow][move.fromCol] = Piece();
            
            // Update en passant target if pawn moved two squares
            if (movingPiece.type == PieceType::Pawn && abs(move.toRow - move.fromRow) == 2) {
                enPassantTargetRow = (move.fromRow + move.toRow) / 2;
                enPassantTargetCol = move.fromCol;
            } else {
                // Clear en passant target for any other move
                enPassantTargetRow = -1;
                enPassantTargetCol = -1;
            }
            
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

int Board::getEnPassantTargetRow() const {
    return enPassantTargetRow;
}

int Board::getEnPassantTargetCol() const {
    return enPassantTargetCol;
}
