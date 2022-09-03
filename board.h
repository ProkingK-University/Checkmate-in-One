#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

class board
{
private:
    int numWhitePieces;
    int numBlackPieces;

    piece** whitePieces;
    piece** blackPieces;

    std::string move;
    std::string ** chessboard;

    char sideToMove;

    board& operator++();
public:
    board(std::string pieceList);

    board& operator--();

    bool checkIfPieceHasCheck(std::string pieceType, int xPos, int yPos, int kingX, int kingY);

    ~board();
};

#endif