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

    void print()
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                std::cout<< chessboard[i][j] << " ";
            }
            std::cout<<std::endl;
        }
    }
};

#endif