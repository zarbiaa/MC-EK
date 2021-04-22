#include <iostream>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

using namespace std;

void f(){
    int n_obs = 100000;
    int max_degree_ = 2;
    //double xy[n_obs][max_degree_ + 2];

    double* xy = new double[n_obs * (max_degree_ + 2)];
    cout << "here" << endl;

}
int main() {
    //PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //pnl_rng_sseed(rng, time(NULL));
    PnlVect *v = pnl_vect_create(5);
    pnl_vect_print(v);
    std::cout << v->size << std::endl;
    cout << "before function call" << endl;
    f();
    cout << "after function call" << endl;

    return 0;
}
