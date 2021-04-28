//
// Created by amine on 4/28/2021.
//
#include <iostream>
#include <string>
#include <ctime>
#include "../src/AmericanMonteCarlo.hpp"

using namespace std;

int main(){
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double r = 0.06; // 0.01 %
    double spot_ = 38;
    double sigma_ = 0.2;
    int size = 1;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 1.0;
    double rho = 0;
    double strike = 40;
    int nbTimeSteps = 50;
    int nb_paths = 100000;
    string poly_type = "Laguerre";
    int max_degree = 2;
    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    VanillaPutOption* putOption = new VanillaPutOption(strike, T);
    PolyReg* mod_reg = new PolyReg(poly_type, max_degree);
    AmericanMonteCarlo* amc = new AmericanMonteCarlo(model, putOption, mod_reg, rng, nb_paths, nbTimeSteps);

    double price = 0.0; PnlVect* deltas = pnl_vect_create(size); PnlVect* antithetic_deltas = pnl_vect_create(size); double antithetic_price = 0.0; double antithetic_delta = 0.0;
    amc->price_and_delta(price, deltas);
    cout << "AMC price: " << price << endl;
    cout << "AMC Deltas: " << endl;
    pnl_vect_print(deltas);

    amc->antithetic_price_and_delta(antithetic_price, antithetic_deltas);
    cout << "antithetic AMC price : " << antithetic_price << endl;
    cout << "antithetic AMC Deltas: " << endl;
    pnl_vect_print(antithetic_deltas);

    pnl_vect_free(&deltas); pnl_vect_free(&antithetic_deltas);
}
