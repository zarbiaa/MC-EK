//
// Created by amine on 3/19/2021.
//
#include <iostream>
#include <string>
#include <ctime>
#include "../src/AmericanMonteCarlo.hpp"

using namespace std;

int main(){
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    /*double r = 0.06; // 0.01 %
    double spot_ = 40;
    double sigma_ = 0.4;
    int size = 1;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 2.0;
    double rho = 0;
    double strike = 40;
    int nbTimeSteps = 2*50;
    int nb_paths = 100000;*/

    double r = 0.04; // 0.01 %
    double spot_ = 1;
    double sigma_ = 0.2;
    int size = 1;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 1.0;
    double rho = 0;
    double strike = 1;
    int nbTimeSteps = 2*50;
    int nb_paths = 100000;
    string poly_type = "Chebyshev2";
    int max_degree = 3;
    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    VanillaPutOption* putOption = new VanillaPutOption(strike, T);
    PolyReg* mod_reg = new PolyReg(poly_type, max_degree);
    AmericanMonteCarlo* amc = new AmericanMonteCarlo(model, putOption, mod_reg, rng, nb_paths, nbTimeSteps);

    PnlMat* exercises_matrix = pnl_mat_create(nb_paths, nbTimeSteps + 1);
    amc->exerciseMatrix(exercises_matrix);

    //cout << "exercise matrix" << endl;
    //pnl_mat_print(exercises_matrix);

    pnl_mat_free(&exercises_matrix);


}