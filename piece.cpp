#include "piece.h"

piece::piece() {}

//This is the copy constuctor
piece::piece(piece* newPiece)
{
    this->xPos = newPiece->getX();
    this->yPos = newPiece->getY();
    this->side = newPiece->getSide();
    this->pieceType = newPiece->getPieceType();
}

//This is the parameter constuctor
piece::piece(std::string pType, char side, int x, int y) : pieceType(pType), side(side), xPos(x), yPos(y) {}

//Setters for X and Y coordinates
void piece::setX(int x) {xPos = x;}
void piece::setY(int y) {yPos = y;}

//Getters for X and Y coordinates`
int piece::getX() {return xPos;}
int piece::getY() {return yPos;}

char piece::getSide() {return side;}

std::string piece::getPieceType() {return pieceType;}

//This is an overloaded subscript operator where 0 or 1 returns the x or y coordinate respectively 
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

//This is an overloaded addition operator where the coordintes are changed
piece& piece::operator+(std::string move)
{
    if (((int)move[0]-48 >= 0 && (int)move[0]-48 <= 7) && ((int)move[2]-48 >= 0 && (int)move[2]-48 <= 7))
    {
        xPos = (int)move[0]-48;
        yPos = (int)move[2]-48;
    }

    return *this;
}

//The overloaded output operator prints out the coordinates
std::ostream& operator<<(std::ostream& output, const piece& t)
{
    output<< t.side << " " << t.pieceType << " at [" << t.xPos << "," << t.yPos << "]" <<std::endl;

    return output;
}

//The destructor shows the piece that has been deleted
piece::~piece()
{
    std::cout<< "(" << xPos << "," << yPos << ")" << " " << side << " " << pieceType << " deleted" <<std::endl;
}

// ;)