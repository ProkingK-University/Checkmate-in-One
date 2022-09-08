#include <fstream>
#include <sstream>

#include "board.h"

board::board(std::string pieceList)
{
    //Declear all variables
    numWhitePieces = 0;
    numBlackPieces = 0;

    int w = 0;
    int b = 0;

    std::string line = "";

    std::string text1 = "";
    std::string text2 = "";
    std::string text3 = "";
    std::string text4 = "";

    //Instantiating the chessboard
    chessboard = new std::string*[8];

    for (int i = 0; i < 8; i++)
    {
        chessboard[i] = new std::string[8];
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            chessboard[i][j] = "-";
        }
    }
    
    //Populating the chessboard
    std::ifstream file(pieceList);

    if(!file.is_open())
    {
        std::cout<< "File failed to open" <<std::endl;
    }

    getline(file, line);
    getline(file, line);

    while (getline(file, line))
    {
        if (line[0] == 'w')
        {
            numWhitePieces++;
        }
        else
        {
            numBlackPieces++;
        }
    }
    file.close();

    whitePieces = new piece*[numWhitePieces];
    blackPieces = new piece*[numBlackPieces];

    file.open(pieceList);

    getline(file, line);
    sideToMove = line[0];

    getline(file, line);
    move = line;

    while (getline(file, line))
    {
        std::stringstream ss(line);

        getline(ss, text1, ',');
        char side = text1[0];

        getline(ss, text2, ',');
        std::string pieceType = text2;

        getline(ss, text3, ',');
        int x = stoi(text3);

        getline(ss, text4, ',');
        int y = stoi(text4);

        if (side == 'w')
        {
            whitePieces[w] = new piece(pieceType, side, x, y);

            w++;
        }
        else
        {
            blackPieces[b] = new piece(pieceType, side, x, y);

            b++;
        }
        
        if (pieceType == "king")
        {
            chessboard[y][x] = text1+"K";
        }
        else
        {
            chessboard[y][x] = text1+pieceType[0];
        }
    }
    file.close();
}

board& board::operator++()
{
    //Moving the piece in the set of coords
    int oldX = (int)move[0]-48;
    int oldY = (int)move[2]-48;
    int newX = (int)move[4]-48;
    int newY = (int)move[6]-48;                                                                             

    if (sideToMove == 'w')
    {
        //Check white pieces to move
        for (int i = 0; i < numWhitePieces; i++)
        {
            if (whitePieces[i]->getX() == oldX && whitePieces[i]->getY() == oldY)
            {
                chessboard[oldY][oldX] = "-";

                if (whitePieces[i]->getPieceType() == "king")
                {
                    chessboard[newY][newX] = "wK";
                }
                else
                {
                    std::string s = "w";
                    std::string ss = whitePieces[i]->getPieceType();
                    chessboard[newY][newX] = s+ss[0];
                }

                whitePieces[i]->setX(newX);
                whitePieces[i]->setY(newY);

                return *this;
            }
        }
    }
    else
    {
        //Check black pieces to move
        for (int i = 0; i < numBlackPieces; i++)
        {
            if (blackPieces[i]->getX() == oldX && blackPieces[i]->getY() == oldY)
            {
                chessboard[oldY][oldX] = "-";

                if (blackPieces[i]->getPieceType() == "king")
                {
                    chessboard[newY][newX] = "bK";
                }
                else
                {
                    std::string s = "b";
                    std::string ss = whitePieces[i]->getPieceType();
                    chessboard[newY][newX] = s+ss[0];                
                }

                blackPieces[i]->setX(newX);
                blackPieces[i]->setY(newY);

                return *this;
            }
        }
    }

    return *this;
}

board& board::operator--()
{
    //Move piece
    ++*this;

    int x = 0;
    int y = 0;

    int newX = (int)move[4]-48;
    int newY = (int)move[6]-48;

    if (sideToMove == 'w')
    {
        for (int i = 0; i < numBlackPieces; i++)
        {
            if (blackPieces[i]->getPieceType() == "king")
            {
                x = blackPieces[i]->getX();
                y = blackPieces[i]->getY();
                
                //Check if king move is valid
                if ((blackPieces[i]->getX()-1 >=0 && blackPieces[i]->getX()-1 < 8) && (blackPieces[i]->getY()-1 >=0 && blackPieces[i]->getY()-1 < 8) && chessboard[blackPieces[i]->getY()-1][blackPieces[i]->getX()-1] == "-")
                {
                    //std::cout<< "1" <<std::endl;
                    //Move king
                    blackPieces[i]->setX(blackPieces[i]->getX()-1);
                    blackPieces[i]->setY(blackPieces[i]->getY()-1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getY() == newY)
                        {
                            //Check if king avoied checkmate
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (blackPieces[i]->getY()-1 >=0 && blackPieces[i]->getY()-1 < 8 && chessboard[blackPieces[i]->getY()-1][blackPieces[i]->getX()] == "-")
                {
                    blackPieces[i]->setY(blackPieces[i]->getY()-1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if ((blackPieces[i]->getX()+1 >=0 && blackPieces[i]->getX()+1 < 8) && (blackPieces[i]->getY()-1 >=0 && blackPieces[i]->getY()-1 < 8) && chessboard[blackPieces[i]->getY()-1][blackPieces[i]->getX()+1] == "-")
                {
                    blackPieces[i]->setX(blackPieces[i]->getX()+1);
                    blackPieces[i]->setY(blackPieces[i]->getY()-1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                            
                        }
                    }
                }
                else if (blackPieces[i]->getX()+1 >= 0 && blackPieces[i]->getX()+1 < 8 && chessboard[blackPieces[i]->getY()][blackPieces[i]->getX()+1] == "-")
                {
                    blackPieces[i]->setX(blackPieces[i]->getX()+1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                            
                        }
                    }
                }
                else if ((blackPieces[i]->getX()+1 >=0 && blackPieces[i]->getX()+1 < 8) && (blackPieces[i]->getY()+1 >= 0 && blackPieces[i]->getY()+1 < 8) && chessboard[blackPieces[i]->getY()+1][blackPieces[i]->getX()+1] == "-")
                {
                    blackPieces[i]->setX(blackPieces[i]->getX()+1);
                    blackPieces[i]->setY(blackPieces[i]->getY()+1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if ((whitePieces[j]->getX() == newX) && (whitePieces[j]->getY() == newY))
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (blackPieces[i]->getY()+1 >=0 && blackPieces[i]->getY()+1 < 8 && chessboard[blackPieces[i]->getY()+1][blackPieces[i]->getX()] == "-")
                {
                    blackPieces[i]->setY(blackPieces[i]->getY()+1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getX() == newY)
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if ((blackPieces[i]->getX()-1 >=0 && blackPieces[i]->getX()-1 < 8) && (blackPieces[i]->getY()+1 >=0 && blackPieces[i]->getY()+1 < 8) && chessboard[blackPieces[i]->getY()+1][blackPieces[i]->getX()-1] == "-")
                {
                    blackPieces[i]->setX(blackPieces[i]->getX()-1);
                    blackPieces[i]->setY(blackPieces[i]->getY()+1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getX() == newY)
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (blackPieces[i]->getX()-1 >=0 && blackPieces[i]->getX()-1 < 8 && chessboard[blackPieces[i]->getY()][blackPieces[i]->getX()-1] == "-")
                {
                    blackPieces[i]->setX(blackPieces[i]->getX()-1);

                    for (int j = 0; j < numWhitePieces; j++)
                    {
                        if (whitePieces[j]->getX() == newX && whitePieces[j]->getX() == newY)
                        {
                            if (!checkIfPieceHasCheck(whitePieces[j]->getPieceType(), newX, newY, blackPieces[i]->getX(), blackPieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of b King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else
                {
                    std::cout<< "Success: Checkmate of b King at [" << x << "," << y << "]" <<std::endl;
                    
                    return *this;
                }

                break;
            }
        }
    }
    else
    {
        //Find white king
        for (int i = 0; i < numWhitePieces; i++)
        {
            if (whitePieces[i]->getPieceType() == "king")
            {
                x = whitePieces[i]->getX();
                y = whitePieces[i]->getY();

                //Check if king move is valid
                if ((whitePieces[i]->getX()-1 >=0 && whitePieces[i]->getX()-1 < 8) && (whitePieces[i]->getY()-1 >=0 && whitePieces[i]->getY()-1 < 8) && chessboard[whitePieces[i]->getY()-1][whitePieces[i]->getX()-1] == "-")
                {
                    //Move king
                    whitePieces[i]->setX(whitePieces[i]->getX()-1);
                    whitePieces[i]->setY(whitePieces[i]->getY()-1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            //Check for checkmate
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (whitePieces[i]->getY()-1 >=0 && whitePieces[i]->getY()-1 < 8 && chessboard[whitePieces[i]->getY()-1][whitePieces[i]->getX()] == "-")
                {
                    whitePieces[i]->setY(whitePieces[i]->getY()-1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if ((whitePieces[i]->getX()+1 >=0 && whitePieces[i]->getX()+1 < 8) && (whitePieces[i]->getY()-1 >=0 && whitePieces[i]->getY()-1 < 8) && chessboard[whitePieces[i]->getY()-1][whitePieces[i]->getX()+1] == "-")
                {
                    whitePieces[i]->setX(whitePieces[i]->getX()+1);
                    whitePieces[i]->setY(whitePieces[i]->getY()-1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (whitePieces[i]->getX()+1 >=0 && whitePieces[i]->getX()+1 < 8 && chessboard[whitePieces[i]->getY()][whitePieces[i]->getX()+1] == "-")
                {
                    whitePieces[i]->setX(whitePieces[i]->getX()+1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if ((whitePieces[i]->getX()+1 >=0 && whitePieces[i]->getX()+1 < 8) && (whitePieces[i]->getY()+1 >=0 && whitePieces[i]->getY()+1 < 8) && chessboard[whitePieces[i]->getY()+1][whitePieces[i]->getX()+1] == "-")
                {
                    whitePieces[i]->setX(whitePieces[i]->getX()+1);
                    whitePieces[i]->setY(whitePieces[i]->getY()+1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (whitePieces[i]->getY()+1 >=0 && whitePieces[i]->getY()+1 < 8 && chessboard[whitePieces[i]->getY()+1][whitePieces[i]->getX()] == "-")
                {
                    whitePieces[i]->setY(whitePieces[i]->getY()+1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if ((whitePieces[i]->getX()-1 >=0 && whitePieces[i]->getX()-1 < 8) && (whitePieces[i]->getY()+1 >=0 && whitePieces[i]->getY()+1 < 8) && chessboard[whitePieces[i]->getY()+1][whitePieces[i]->getX()-1] == "-")
                {
                    whitePieces[i]->setX(whitePieces[i]->getX()-1);
                    whitePieces[i]->setY(whitePieces[i]->getY()+1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else if (whitePieces[i]->getX()-1 >=0 && whitePieces[i]->getX()-1 < 8 && chessboard[whitePieces[i]->getY()][whitePieces[i]->getX()-1] == "-")
                {
                    whitePieces[i]->setX(whitePieces[i]->getX()-1);

                    for (int j = 0; j < numBlackPieces; j++)
                    {
                        if (blackPieces[j]->getX() == newX && blackPieces[j]->getY() == newY)
                        {
                            if (!checkIfPieceHasCheck(blackPieces[j]->getPieceType(), newX, newY, whitePieces[i]->getX(), whitePieces[i]->getY()));
                            {
                                std::cout<< "Failed: No Checkmate of w King" <<std::endl;
                                
                                return *this;
                            }
                        }
                    }
                }
                else
                {
                    std::cout<< "Success: Checkmate of b King at [" << x << "," << y << "]" <<std::endl;

                    return *this;
                }
            }
        }
    }

    return *this;
}

bool board::checkIfPieceHasCheck(std::string pieceType, int xPos, int yPos, int kingX, int kingY)
{
    //Checkmate logic for all piece types
    if (pieceType == "pawn")
    {
        if ((kingX == xPos-1 && kingY == yPos-1) || (kingX == xPos+1 && kingY == yPos-1))
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }
    else if (pieceType == "knight")
    {
        if ((kingX == xPos-2 && kingY == yPos-1) || 
            (kingX == xPos+1 && kingY == yPos-2) ||
            (kingX == xPos+2 && kingY == yPos-1) ||
            (kingX == xPos+2 && kingY == yPos+1) ||
            (kingX == xPos+1 && kingY == yPos+2) ||
            (kingX == xPos-1 && kingY == yPos+2) ||
            (kingX == xPos-2 && kingY == yPos+1) ||
            (kingX == xPos-1 && kingY == yPos-2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (pieceType == "rook")
    {
        if (kingX == xPos || kingY == yPos)
        {
            int distanceX = xPos-kingX;
            int distanceY = yPos-kingY;
            
            for (int i = 0; i < numWhitePieces; i++)
            {
                if (distanceX == abs(distanceX))
                {
                    if (whitePieces[i]->getX() < xPos && whitePieces[i]->getX() > kingX && whitePieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (whitePieces[i]->getX() > xPos && whitePieces[i]->getX() < kingX && whitePieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }

                if (distanceY == abs(distanceY))
                {
                    if (whitePieces[i]->getY() < yPos && whitePieces[i]->getY() > kingY && whitePieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (whitePieces[i]->getY() > yPos && whitePieces[i]->getY() < kingY && whitePieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
            }
            
            for (int i = 0; i < numBlackPieces; i++)
            {
                if (distanceX == abs(distanceX))
                {
                    if (blackPieces[i]->getX() < xPos && blackPieces[i]->getX() > kingX && blackPieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (blackPieces[i]->getX() > xPos && blackPieces[i]->getX() < kingX && blackPieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }

                if (distanceY == abs(distanceY))
                {
                    if (blackPieces[i]->getY() < yPos && blackPieces[i]->getY() > kingY && blackPieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (blackPieces[i]->getY() > yPos && blackPieces[i]->getY() < kingY && blackPieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
            }
            
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (pieceType == "bishop")
    {
        if (abs(kingX-xPos) == abs(kingY-yPos))
        {
            for (int i = 0; i < numWhitePieces; i++)
            {
                if (kingX<xPos && kingY>yPos)
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()<xPos && whitePieces[i]->getY()>yPos) && (whitePieces[i]->getX()>kingX && whitePieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY>yPos)
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()>xPos && whitePieces[i]->getY()>yPos) && (whitePieces[i]->getX()<kingX && whitePieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY<yPos)
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()>xPos && whitePieces[i]->getY()<yPos) && (whitePieces[i]->getX()<kingX && whitePieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
                else
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()<xPos && whitePieces[i]->getY()<yPos) && (whitePieces[i]->getX()>kingX && whitePieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
            }

            for (int i = 0; i < numBlackPieces; i++)
            {
                if (kingX<xPos && kingY>yPos)
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()<xPos && blackPieces[i]->getY()>yPos) && (blackPieces[i]->getX()>kingX && blackPieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY>yPos)
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()>xPos && blackPieces[i]->getY()>yPos) && (blackPieces[i]->getX()<kingX && blackPieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY<yPos)
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()>xPos && blackPieces[i]->getY()<yPos) && (blackPieces[i]->getX()<kingX && blackPieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
                else
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()<xPos && blackPieces[i]->getY()<yPos) && (blackPieces[i]->getX()>kingX && blackPieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        return false;
    }
    else if (pieceType == "queen")
    {
        if (kingX == xPos || kingY == yPos)
        {
            int distanceX = xPos-kingX;
            int distanceY = yPos-kingY;

            for (int i = 0; i < numWhitePieces; i++)
            {
                if (distanceX == abs(distanceX))
                {
                    if (whitePieces[i]->getX() < xPos && whitePieces[i]->getX() > kingX && whitePieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (whitePieces[i]->getX() > xPos && whitePieces[i]->getX() < kingX && whitePieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }

                if (distanceY == abs(distanceY))
                {
                    if (whitePieces[i]->getY() < yPos && whitePieces[i]->getY() > kingY && whitePieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (whitePieces[i]->getY() > yPos && whitePieces[i]->getY() < kingY && whitePieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
            }
            
            for (int i = 0; i < numBlackPieces; i++)
            {
                if (distanceX == abs(distanceX))
                {
                    if (blackPieces[i]->getX() < xPos && blackPieces[i]->getX() > kingX && blackPieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (blackPieces[i]->getX() > xPos && blackPieces[i]->getX() < kingX && blackPieces[i]->getY() == yPos)
                    {
                        return false;
                    }
                }

                if (distanceY == abs(distanceY))
                {
                    if (blackPieces[i]->getY() < yPos && blackPieces[i]->getY() > kingY && blackPieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
                else
                {
                    if (blackPieces[i]->getY() > yPos && blackPieces[i]->getY() < kingY && blackPieces[i]->getX() == xPos)
                    {
                        return false;
                    }
                }
            }
            
            return true;
        }
        else if (abs(kingX-xPos) == abs(kingY-yPos))
        {
            for (int i = 0; i < numWhitePieces; i++)
            {
                if (kingX<xPos && kingY>yPos)
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()<xPos && whitePieces[i]->getY()>yPos) && (whitePieces[i]->getX()>kingX && whitePieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY>yPos)
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()>xPos && whitePieces[i]->getY()>yPos) && (whitePieces[i]->getX()<kingX && whitePieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY<yPos)
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()>xPos && whitePieces[i]->getY()<yPos) && (whitePieces[i]->getX()<kingX && whitePieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
                else
                {
                    if (abs(whitePieces[i]->getX()-xPos) == abs(whitePieces[i]->getY()-yPos) && (whitePieces[i]->getX()<xPos && whitePieces[i]->getY()<yPos) && (whitePieces[i]->getX()>kingX && whitePieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
            }

            for (int i = 0; i < numBlackPieces; i++)
            {
                if (kingX<xPos && kingY>yPos)
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()<xPos && blackPieces[i]->getY()>yPos) && (blackPieces[i]->getX()>kingX && blackPieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY>yPos)
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()>xPos && blackPieces[i]->getY()>yPos) && (blackPieces[i]->getX()<kingX && blackPieces[i]->getY()<kingY))
                    {
                        return false;
                    }
                }
                else if (kingX>xPos && kingY<yPos)
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()>xPos && blackPieces[i]->getY()<yPos) && (blackPieces[i]->getX()<kingX && blackPieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
                else
                {
                    if (abs(blackPieces[i]->getX()-xPos) == abs(blackPieces[i]->getY()-yPos) && (blackPieces[i]->getX()<xPos && blackPieces[i]->getY()<yPos) && (blackPieces[i]->getX()>kingX && blackPieces[i]->getY()>kingY))
                    {
                        return false;
                    }
                }
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

board::~board()
{
    int numOfPieces = 0;

    for (int i = 0; i < numWhitePieces; i++)
    {
        if (whitePieces[i] != NULL)
        {
            whitePieces[i] = NULL;
            delete whitePieces[i];
            numOfPieces++;
        }
    }

    whitePieces = NULL;
    delete [] whitePieces;

    for (int i = 0; i < numBlackPieces; i++)
    {
        if (blackPieces != NULL)
        {
            blackPieces[i] = NULL;
            delete blackPieces[i];
            numOfPieces++;
        }
    }

    blackPieces = NULL;
    delete [] blackPieces;

    for (int i = 0; i < 8; i++)
    {
        chessboard[i] = NULL;
        delete [] chessboard[i];
    }

    chessboard = NULL;
    delete [] chessboard;

    std::cout<< "Num Pieces Removed: " << numOfPieces <<std::endl;
}