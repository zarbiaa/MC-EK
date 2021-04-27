//
// Created by amine on 4/26/2021.
//
#include <iostream>
#include <string>
#include <ctime>
#include "../src/AmericanMonteCarlo.hpp"
#include "../src/Option.h"

using namespace std;

int main(){
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double r = 0.06;
    double spot_ = 38;
    double sigma_ = 0.2;
    int size = 2;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 1.0;
    double rho = 0.999;
    double strike = 40;
    int nbTimeSteps = 50;
    int nb_paths = 10000;
    string poly_type = "Laguerre";
    int max_degree = 2;
    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    Option* geo_putOption = new GeometricPutOption(strike, T);
    PolyReg* mod_reg = new PolyReg(poly_type, max_degree);
    AmericanMonteCarlo* amc = new AmericanMonteCarlo(model, geo_putOption, mod_reg, rng, nb_paths, nbTimeSteps);

    PnlMat* exercises_matrix = pnl_mat_create(nb_paths, nbTimeSteps + 1);
    amc->exerciseMatrix(exercises_matrix);

    pnl_mat_free(&exercises_matrix);

}
