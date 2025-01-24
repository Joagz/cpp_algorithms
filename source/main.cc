#include "pathfinder/a_star.hh"

int main() {
    A_star pathfinder = A_star(100, 100);
    pathfinder.run(5,10, 40, 56);

    return 0;
}