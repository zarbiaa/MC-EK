#include <iostream>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

int main() {
    //PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //pnl_rng_sseed(rng, time(NULL));
    PnlVect *v = pnl_vect_create(5);
    pnl_vect_print(v);
    std::cout << v->size << std::endl;


    char* s = "hi";
    std::cout << s << std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
