//
// Created by amine on 3/19/2021.
//

#include <iostream>
#include <string>
#include <ctime>
#include "../src/BlackScholesModel.h"


using namespace std;

int main(){
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double r = 0.0001; // 0.01 %
    double spot_ = 100;
    double sigma_ = 0.15;
    int size = 2;
    double rho = 0.2;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 1.0;
    double strike = 100;
    int nbTimeSteps = 5;

    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    PnlMat* simulated_path = pnl_mat_create(nbTimeSteps + 1, size);
    model->generate_path(simulated_path, T, nbTimeSteps, rng);


    pnl_mat_print(simulated_path);
}
