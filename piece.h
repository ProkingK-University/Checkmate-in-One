#ifndef PIECE_H
#define PIECE_H

#include <iostream>

class piece;

std::ostream& operator<<(std::ostream& output, const piece& t);

class piece
{
private:
    int xPos;
    int yPos;

    char side;

    std::string pieceType;
public:
    piece();
    piece(piece* newPiece);
    piece(std::string pType, char side, int x, int y);

    void setX(int x);
    void setY(int y);

    int getX();
    int getY();

    char getSide();

    std::string getPieceType();

    void operator[](int pos);
    piece& operator+(std::string move);
    friend std::ostream& operator<<(std::ostream& output, const piece& t);

    ~piece();
};

#endif