//
// Created by amine on 4/27/2021.
//
#include "ClosedFormBS_Put.cpp"
#include "../src/AmericanMonteCarlo.hpp"
#include "../src/EuropeanMonteCarlo.hpp"
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

int main(){
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double r = 0.06;
    double spot_ = 36;
    double sigma_ = 0.2;
    int size = 2;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double rho = 0.99999;
    double T = 1.0;
    double strike = 40;

    double BS_price, BS_delta;
    price_european_geometric_put(spot, sigma, rho, strike, T, r, &BS_price, &BS_delta);

    // print the result of the BS european geometric put price & delta
    cout << "BS price at t =0: " << BS_price << endl;
    cout << "BS delta at t =0: " << BS_delta << endl;

    int nbTimeSteps = 50;
    int nb_paths = 100000;

    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    Option* geo_putOption = new GeometricPutOption(strike, T);
    EuropeanMonteCarlo* mc = new EuropeanMonteCarlo(model, geo_putOption, rng, nb_paths, nbTimeSteps);

    double MC_price, ic;

    mc->price(MC_price, ic);

    // print the result of the MC european geometric put price
    cout << "MC price at t =0: " << MC_price << endl;
    cout << "MC confidence interval at t =0: [" << MC_price - ic/2 << "," << MC_price + ic/2 << "]" << endl;

    string poly_type = "Laguerre";
    int max_degree = 2;
    PolyReg* mod_reg = new PolyReg(poly_type, max_degree);
    AmericanMonteCarlo* amc = new AmericanMonteCarlo(model, geo_putOption, mod_reg, rng, nb_paths, nbTimeSteps);
    PnlMat* exercises_matrix = pnl_mat_create(nb_paths, nbTimeSteps + 1);
    amc->exerciseMatrix(exercises_matrix);

    pnl_mat_free(&exercises_matrix);


}

