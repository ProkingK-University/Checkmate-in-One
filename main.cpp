#include "solver.h"

int main()
{
    piece p("bishop", 'b', 2, 3);

    std::cout<<p;

    p[1];

    p = p + "2,3";

    std::cout<<p;


    board b("game1.txt");
    b.print();

    std::cout<<std::endl;

    --b;

    solver s("games.txt");

    return 0;
}