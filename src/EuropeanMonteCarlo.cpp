//
// Created by amine on 4/27/2021.
//

#include "EuropeanMonteCarlo.hpp"

void EuropeanMonteCarlo::retrieve_stock_values(PnlMat** stock_paths, int idx_instant) {
    for (int idx = 0; idx < mod_->size_; idx++) {
        pnl_mat_get_col(V_, stock_paths[idx], idx_instant);
        pnl_mat_set_col(mat_stock_values_, V_, idx);
    }
}

double EuropeanMonteCarlo::compute_DF(int present_instant, int future_instant) {
    double timespan = opt_->maturity_ / (float) nb_timesteps_;
    return exp(-mod_->r_ * (future_instant - present_instant) * timespan);
}


void EuropeanMonteCarlo::Compute_payoffs(PnlMat** stock_paths, PnlVect* payoffs, int idx_instant) {
    retrieve_stock_values(stock_paths, idx_instant);
    opt_->payoff(payoffs, mat_stock_values_);
}

void EuropeanMonteCarlo::price(double &price, double &ic){
    PnlVect* tmp_payoffs = pnl_vect_create(nb_paths_);
    double timespan = opt_->maturity_ / (float) nb_timesteps_;
    PnlMat** stock_paths = (PnlMat**) malloc(0);
    for(int i = 0; i < mod_->size_; i++){
        stock_paths[i] = pnl_mat_create(nb_paths_, nb_timesteps_ + 1);
    }
    mod_->generate_paths(stock_paths, opt_->maturity_, nb_paths_, nb_timesteps_, rng_);
    Compute_payoffs(stock_paths, tmp_payoffs, nb_timesteps_);
    double DF = compute_DF(0, nb_timesteps_);
    double payoffs = pnl_vect_sum(tmp_payoffs) / nb_paths_;
    double payoffs_squared = pnl_vect_scalar_prod(tmp_payoffs, tmp_payoffs) / nb_paths_;
    double var = pow(DF, 2) * (payoffs_squared - pow(payoffs, 2));
    price = DF *  payoffs;

    // width of the 95% MC confidence interval
    ic = 2 * 1.96 * sqrt(var) / sqrt(nb_paths_);
    for(int i = 0; i < mod_->size_; i++){
        pnl_mat_free(&stock_paths[i]);
    }
    free(stock_paths);
    pnl_vect_free(&tmp_payoffs);
}
