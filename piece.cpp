#include "piece.h"

piece::piece() {}

piece::piece(piece* newPiece)
{
    this->xPos = newPiece->getX();
    this->yPos = newPiece->getY();
    this->side = newPiece->getSide();
    this->pieceType = newPiece->getPieceType();
}

piece::piece(std::string pType, char side, int x, int y) : pieceType(pType), side(side), xPos(x), yPos(y) {}

void piece::setX(int x) {xPos = x;}
void piece::setY(int y) {yPos = y;}

int piece::getX() {return xPos;}
int piece::getY() {return yPos;}

char piece::getSide() {return side;}

std::string piece::getPieceType() {return pieceType;}

void piece::operator[](int pos)
{
    if (pos == 0 || pos == 1)
    {
        if (pos == 0)
        {
            std::cout<< "x coord: " << xPos <<std::endl;
        }
        else
        {
            std::cout<< "y coord: " << yPos <<std::endl;
        }
    }
    else
    {
        std::cout<< "Invalid Index" <<std::endl;
    }
}

piece& piece::operator+(std::string move)
{
    if (((int)move[0]-48 >= 0 && (int)move[0]-48 <= 7) && ((int)move[2]-48 >= 0 && (int)move[2]-48 <= 7))
    {
        xPos = (int)move[0]-48;
        yPos = (int)move[2]-48;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& output, const piece& t)
{
    output<< t.side << " " << t.pieceType << " at [" << t.xPos << "," << t.yPos << "]" <<std::endl;

    return output;
}

piece::~piece()
{
    std::cout<< "(" << xPos << "," << yPos << ")" << " " << side << " " << pieceType << " deleted" <<std::endl;
}

// ;)