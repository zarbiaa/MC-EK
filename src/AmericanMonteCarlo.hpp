//
// Created by amine on 4/20/2021.
//

#ifndef MC_EK_AMERICANMONTECARLO_HPP
#define MC_EK_AMERICANMONTECARLO_HPP

#include "Option.h"
#include "VanillaPutOption.hpp"
#include "PolyReg.h"
#include "BlackScholesModel.h"
#include "pnl/pnl_random.h"

class AmericanMonteCarlo {
public:
    BlackScholesModel* mod_;
    Option* opt_;
    PolyReg* mod_reg_;
    PnlRng* rng_;
    int nb_paths_;
    int nb_timesteps_;
    PnlMat* mat_stock_values_;
    PnlMat* indic_stock_values_;
    PnlVect* tmp_CFs_;
    PnlVect* tmp_future_CF_;
    PnlVect *V_;
    PnlVect *vect_stock_values_1;
    PnlVect *vect_stock_values_2;

    ~AmericanMonteCarlo(){
        delete mod_; delete mod_reg_; delete opt_;
        pnl_rng_free(&rng_);
        pnl_mat_free(&mat_stock_values_); pnl_mat_free(&indic_stock_values_);
        pnl_vect_free(&V_); pnl_vect_free(&vect_stock_values_1); pnl_vect_free(&vect_stock_values_2);
        pnl_vect_free(&tmp_CFs_); pnl_vect_free(&tmp_future_CF_);
    }
    AmericanMonteCarlo(BlackScholesModel* mod, Option* opt, PolyReg* mod_reg, PnlRng* rng, int M, int N){
        mod_ = mod;
        opt_ = opt;
        mod_reg_ = mod_reg;
        rng_ = rng;
        nb_paths_ = M;
        nb_timesteps_ = N;
        mat_stock_values_ = pnl_mat_create(nb_paths_, mod_->size_);
        indic_stock_values_ = pnl_mat_create(nb_paths_, mod_->size_);
        vect_stock_values_1 = pnl_vect_create(nb_paths_);
        vect_stock_values_2 = pnl_vect_create(nb_paths_);
        tmp_CFs_ = pnl_vect_create(nb_paths_);
        tmp_future_CF_ = pnl_vect_create(nb_paths_);
        V_ = pnl_vect_create(nb_paths_);
    }

    void Compute_payoffs(PnlMat *exercises_matrix, PnlMat** stock_paths, PnlVect* payoffs, int idx_instant);
    void exerciseMatrix(PnlMat* exercises_matrix);
    void retrieve_stock_values(PnlMat** stock_paths, int idx_instant);
    void retrieve_ATM_stock_values(PnlMat* vect_strike);
    void compute_future_discounted_CF(PnlMat* exercises_matrix, int idx_instant);
    double compute_DF(int present_instant, int future_instant);
    void estimate_continuation(PnlVect* continuation_values);
    void optimal_exercise(PnlMat* exercises_matrix, PnlVect* continuation_values, PnlVect* payoffs, PnlVect* optimal_vect, int idx_instant);
    void optimal_exercise_vect(PnlVect* optimal_vect, PnlVect* payoffs, PnlVect* continuations);

    };


#endif //MC_EK_AMERICANMONTECARLO_HPP
