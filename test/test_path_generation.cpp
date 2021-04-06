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
    double sigma_ = 0.3;
    int size = 2;
    double rho = -1;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 1.0;
    double strike = 100;
    int nbTimeSteps = 8;
    int nb_paths = 10;

    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    PnlMat* simulated_path = pnl_mat_create(nbTimeSteps + 1, size);
    model->generate_path(simulated_path, T, nbTimeSteps, rng);

    cout << "generate several simulations of underlying stocks" << endl;
    PnlMat** stock_paths = (PnlMat**) malloc(0);
    for(int i = 0; i < size; i++){
        stock_paths[i] = pnl_mat_create(nb_paths, nbTimeSteps + 1);
    }

    model->generate_paths(stock_paths, T, nb_paths, nbTimeSteps, rng);
    cout << "final output" << endl;
    for(int i = 0; i < size; i++){
        pnl_mat_print(stock_paths[i]);
        cout << endl;
    }

    pnl_mat_print(pnl_mat_transpose(simulated_path));
}
