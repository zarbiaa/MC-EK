//
// Created by amine on 4/27/2021.
//

#ifndef MC_EK_EUROPEANMONTECARLO_HPP
#define MC_EK_EUROPEANMONTECARLO_HPP

#include "Option.h"
#include "VanillaPutOption.hpp"
#include "GeometricPutOption.hpp"
#include "BlackScholesModel.h"
#include "pnl/pnl_random.h"

class EuropeanMonteCarlo {
public:
    BlackScholesModel* mod_;
    Option* opt_;
    PnlRng* rng_;
    int nb_paths_;
    int nb_timesteps_;
    PnlMat* mat_stock_values_;
    PnlVect *V_;



    ~EuropeanMonteCarlo(){
        delete mod_, delete opt_;
        pnl_rng_free(&rng_); pnl_mat_free(&mat_stock_values_); pnl_vect_free(&V_);
    }

    EuropeanMonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, int M, int N){
        mod_ = mod;
        opt_ = opt;
        rng_ = rng;
        nb_paths_ = M;
        nb_timesteps_ = N;
        mat_stock_values_ = pnl_mat_create(nb_paths_, mod_->size_);
        V_ = pnl_vect_create(nb_paths_);
    }
    void retrieve_stock_values(PnlMat** stock_paths, int idx_instant);
    double compute_DF(int present_instant, int future_instant);
    void Compute_payoffs(PnlMat** stock_paths, PnlVect* payoffs, int idx_instant);
    void price(double &price, double &ic);
};


#endif //MC_EK_EUROPEANMONTECARLO_HPP
