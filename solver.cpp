#include <fstream>
#include <sstream>

#include "solver.h"

solver::solver(std::string games)
{
    std::string line;
    std::string data;
    
    std::ifstream file(games);

    if(!file.is_open())
    {
        std::cout<< "File failed to open" <<std::endl;
    }

    numGames = 0;

    while (getline(file, line)) {numGames++;}

    file.close();

    int order[numGames][2];

    boards = new board*[numGames];

    file.open(games);

    for (int i = 0; i < numGames; i++)
    {
        getline(file, line);

        std::stringstream ss(line);

        getline(ss, data, ',');
        boards[i] = new board(data);

        getline(ss, data, ',');
        order[i][0] = i;
        order[i][1] = stoi(data);
    }

    file.close();

    int i, k, l, j;

    for (i = 1; i < numGames; i++)
    { 
        k = order[i][1];
        l = order[i][0];
        j = i - 1; 

        while (j >= 0 && order[j][1] > k)
        { 
            order[j + 1][1] = order[j][1];
            order[j + 1][0] = order[j][0]; 
            j = j - 1; 
        } 
        order[j + 1][1] = k;
        order[j + 1][0] = l;
    } 

    for (int i = 0; i < numGames; i++)
    {
        --*boards[order[i][0]];
    }  
}

solver::~solver()
{
    for (int i = 0; i < numGames; i++)
    {
        delete boards[i];
        boards[i] = NULL;
    }

    delete [] boards;
    boards = NULL;

    std::cout<< "Num Boards Deleted: " << numGames <<std::endl;
    
}