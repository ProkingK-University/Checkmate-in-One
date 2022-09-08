#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"

class solver
{
private:
    int numGames;

    board** boards;
public:
    solver(std::string games);
    ~solver();
};

#endif