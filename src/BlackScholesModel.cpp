//
// Created by amine on 3/16/2021.
//

#include "BlackScholesModel.h"
#include <math.h>
#include <ctime>
#include <iostream>

using namespace std;


BlackScholesModel::~BlackScholesModel() {
    pnl_vect_free(&sigma_);
    pnl_vect_free(&spot_);
    pnl_mat_free(&correlatioMatrix);
    pnl_mat_free(&G);
    pnl_vect_free(&Ld);
    pnl_vect_free(&Gi);
}

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, int nbTimeSteps) {
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = sigma;
    spot_ = spot;
    correlatioMatrix = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int diag =0; diag < size_; diag++)
        pnl_mat_set(correlatioMatrix, diag, diag, 1);
    pnl_mat_chol(correlatioMatrix);
    G = pnl_mat_create(nbTimeSteps, size_);
    Ld = pnl_vect_create(size_);
    Gi = pnl_vect_create(size);
}

void BlackScholesModel::generate_path(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {
    pnl_mat_set_row(path, spot_, 0);
    pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);

    double expo = 0;
    double timeSpan = T/nbTimeSteps;

    for(int d = 0; d < size_; d++){
        pnl_mat_get_row(Ld, correlatioMatrix, d);
        for(int i = 1; i <= nbTimeSteps; i++){
            pnl_mat_get_row(Gi, G, i - 1);
            expo = pnl_expm1( (r_ - pnl_pow_i(GET(sigma_, d), 2) / 2)  * timeSpan +
                              GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi) ) + 1;
            pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * expo);
        }
    }
}

void BlackScholesModel::generate_paths(PnlMat** stock_paths, double T, int M, int nbTimeSteps, PnlRng *rng) {
    double expo = 0;
    double timeSpan = T/nbTimeSteps;

    // init values for the different spots
    for(int i = 0; i < size_; i++){
        for(int j = 0; j < M; j++){
            pnl_mat_set(stock_paths[i], j, 0, GET(spot_, i));
        }
    }
    for (int idx_path = 0; idx_path < M; idx_path++){
        pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);
        for (int d = 0; d < size_; d++){
            pnl_mat_get_row(Ld, correlatioMatrix, d);
            for (int i = 1; i <= nbTimeSteps; i++) {
                pnl_mat_get_row(Gi, G, i - 1);
                expo = pnl_expm1((r_ - pnl_pow_i(GET(sigma_, d), 2) / 2) * timeSpan +
                                 GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
                pnl_mat_set(stock_paths[d], idx_path, i, pnl_mat_get(stock_paths[d], idx_path, i - 1) * expo);
            }
        }
    }
}

void BlackScholesModel::generate_antithetic_paths(PnlMat** stock_paths, PnlMat** antithetic_stock_paths, double T, int M, int nbTimeSteps, PnlRng *rng){
    double expo = 0;
    double timeSpan = T/nbTimeSteps;

    // init values for the different spots
    for(int i = 0; i < size_; i++){
        for(int j = 0; j < M; j++){
            pnl_mat_set(stock_paths[i], j, 0, GET(spot_, i));
            pnl_mat_set(antithetic_stock_paths[i], j, 0, GET(spot_, i));
        }
    }
    for (int idx_path = 0; idx_path < M; idx_path++){
        pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);
        for (int d = 0; d < size_; d++){
            pnl_mat_get_row(Ld, correlatioMatrix, d);
            for (int i = 1; i <= nbTimeSteps; i++) {
                // fill the stock paths
                pnl_mat_get_row(Gi, G, i - 1);
                expo = pnl_expm1((r_ - pnl_pow_i(GET(sigma_, d), 2) / 2) * timeSpan +
                                 GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
                pnl_mat_set(stock_paths[d], idx_path, i, pnl_mat_get(stock_paths[d], idx_path, i - 1) * expo);
                // fill the antithetic stock paths
                pnl_vect_mult_scalar(Gi, -1.0);
                expo = pnl_expm1((r_ - pnl_pow_i(GET(sigma_, d), 2) / 2) * timeSpan +
                                 GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
                pnl_mat_set(antithetic_stock_paths[d], idx_path, i, pnl_mat_get(antithetic_stock_paths[d], idx_path, i - 1) * expo);
            }
        }
    }

}

