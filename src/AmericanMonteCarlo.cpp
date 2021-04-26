//
// Created by amine on 4/20/2021.
//

#include "AmericanMonteCarlo.hpp"
double static is_bigger(double x, double y){
    return x > y;
}
void AmericanMonteCarlo::optimal_exercise_vect(PnlVect* optimal_vect, PnlVect* payoffs, PnlVect* continuations){
    pnl_vect_map_vect(optimal_vect, payoffs, continuations, is_bigger);
}
void AmericanMonteCarlo::optimal_exercise(PnlMat* exercises_matrix, PnlVect* continuation_values, PnlVect* payoffs, PnlVect* optimal_vect, int idx_instant){
    optimal_exercise_vect(optimal_vect, payoffs, continuation_values);
    pnl_vect_mult_vect_term(payoffs, optimal_vect);
    pnl_mat_set_col(exercises_matrix, payoffs, idx_instant);
    pnl_vect_mult_scalar(optimal_vect, -1); pnl_vect_plus_scalar(optimal_vect, 1);
    for(int idx_future_instant = idx_instant + 1; idx_future_instant <= nb_timesteps_; idx_future_instant++){
        pnl_mat_get_col(tmp_CFs_, exercises_matrix, idx_future_instant);
        pnl_vect_mult_vect_term(tmp_CFs_, optimal_vect);
        pnl_mat_set_col(exercises_matrix, tmp_CFs_, idx_future_instant);
    }
}

void AmericanMonteCarlo::estimate_continuation(PnlVect* continuation_values){
    // here we want to take care of the two cases: 1-d and 2-d
    if(mod_->size_ == 1){
        pnl_mat_get_col(vect_stock_values_1, mat_stock_values_, 0);
        mod_reg_->fit(vect_stock_values_1, tmp_CFs_);
        mod_reg_->apply_fit(vect_stock_values_1, continuation_values);
    }
    else{
        pnl_mat_get_col(vect_stock_values_1, mat_stock_values_, 0);
        pnl_mat_get_col(vect_stock_values_2, mat_stock_values_, 1);
        //cout << "REGRESSION 2-D NEEDED HERE!" << endl;
        // TO DO : manage the 2-d case, fit2 and fit1 !!!
        mod_reg_->fit2(vect_stock_values_1, vect_stock_values_2, tmp_CFs_);
        mod_reg_->apply_fit2(vect_stock_values_1, vect_stock_values_2, continuation_values);
    }
}
double AmericanMonteCarlo::compute_DF(int present_instant, int future_instant){
    double timespan = opt_->maturity_ / (float) nb_timesteps_;
    return exp(-mod_->r_ * (future_instant - present_instant) * timespan);

}
void AmericanMonteCarlo::compute_future_discounted_CF(PnlMat* exercises_matrix, int idx_instant){
    pnl_vect_mult_scalar(tmp_CFs_, 0.0);
    double DF = 0.0;
    for(int idx_future_instant = idx_instant + 1; idx_future_instant <= nb_timesteps_; idx_future_instant++){
        DF = compute_DF(idx_instant, idx_future_instant);
        pnl_mat_get_col(tmp_future_CF_, exercises_matrix, idx_future_instant);
        pnl_vect_mult_scalar(tmp_future_CF_, DF);
        pnl_vect_plus_vect(tmp_CFs_, tmp_future_CF_);
    }
}
void AmericanMonteCarlo::retrieve_ATM_stock_values(PnlMat* vect_strike){
    opt_->compute_indic_ATM(mat_stock_values_, indic_stock_values_, vect_strike);
    pnl_mat_mult_mat_term(mat_stock_values_, indic_stock_values_);
}


void AmericanMonteCarlo::retrieve_stock_values(PnlMat** stock_paths, int idx_instant){
    for(int idx = 0; idx < mod_->size_; idx++){
        pnl_mat_get_col(V_, stock_paths[idx], idx_instant);
        pnl_mat_set_col(mat_stock_values_, V_, idx);
    }

}

void AmericanMonteCarlo::Compute_payoffs(PnlMat *exercises_matrix, PnlMat** stock_paths, PnlVect* payoffs, int idx_instant){
    retrieve_stock_values(stock_paths, idx_instant);
    opt_->payoff(payoffs, mat_stock_values_);
}
void AmericanMonteCarlo::exerciseMatrix(PnlMat *exercises_matrix) {
    PnlVect* tmp_payoffs = pnl_vect_create(nb_paths_);
    PnlVect* indic_ATM = pnl_vect_create(nb_paths_);
    PnlMat* vect_strike = pnl_mat_create_from_scalar(nb_paths_, mod_->size_, opt_->K_);
    PnlVect *tmp_continuation = pnl_vect_create(nb_paths_);
    PnlVect *optimal_vect = pnl_vect_create(nb_paths_);


    double timespan = opt_->maturity_ / (float) nb_timesteps_;

    PnlMat** stock_paths = (PnlMat**) malloc(0);
    for(int i = 0; i < mod_->size_; i++){
        stock_paths[i] = pnl_mat_create(nb_paths_, nb_timesteps_ + 1);
    }
    mod_->generate_paths(stock_paths, opt_->maturity_, nb_paths_, nb_timesteps_, rng_);

    /*double stock_matrix[32] = {1.0, 1.09, 1.08, 1.34, 1.0, 1.16, 1.26, 1.54, 1, 1.22, 1.07, 1.03, 1, 0.93, 0.97, 0.92, 1, 1.11, 1.56, 1.52, 1, 0.76, 0.77, 0.90, 1, 0.92, 0.84, 1.01, 1.0, 0.88, 1.22, 1.34};
    stock_paths[0] = pnl_mat_create_from_ptr(nb_paths_, nb_timesteps_ + 1, &stock_matrix[0]);
    stock_paths[1] = pnl_mat_create(nb_paths_, nb_timesteps_ + 1);*/


    /*for(int i = 0; i < mod_->size_; i++){
        pnl_mat_print(stock_paths[i]);
        cout << endl;
    }*/
    //1st step: fill the last column of exercises_matrix with payoffs at expiry
    Compute_payoffs(exercises_matrix, stock_paths, tmp_payoffs, nb_timesteps_);
    pnl_mat_set_col(exercises_matrix, tmp_payoffs, nb_timesteps_);

    for(int idx_instant = nb_timesteps_ - 1; idx_instant >0; idx_instant--){
        pnl_vect_mult_scalar(tmp_CFs_, 0.0);
        // compute payoffs at instant
        Compute_payoffs(exercises_matrix, stock_paths, tmp_payoffs, idx_instant);
        // retrieve stock prices
        retrieve_stock_values(stock_paths, idx_instant);
        // retrieve ATM stock prices
        retrieve_ATM_stock_values(vect_strike);
        pnl_mat_get_col(indic_ATM, indic_stock_values_, 0);
        // now mat_stock_values contains only ATM stocks
        // now we compute the future discounted CFs
        compute_future_discounted_CF(exercises_matrix, idx_instant);
        // now tmp_CFs is filled with future discounted CFs
        pnl_vect_mult_vect_term(tmp_CFs_, indic_ATM);
        // estimation of continuation using regression
        estimate_continuation(tmp_continuation);
        optimal_exercise(exercises_matrix, tmp_continuation, tmp_payoffs, optimal_vect, idx_instant);
    }


    for(int i = 0; i < mod_->size_; i++){
        pnl_mat_free(&stock_paths[i]);
    }
    free(stock_paths);
    pnl_vect_free(&tmp_payoffs);
    pnl_mat_free(&vect_strike);
    pnl_vect_free(&indic_ATM);
    pnl_vect_free(&tmp_continuation);
    pnl_vect_free(&optimal_vect);

    double price = 0.0;
    double strike = opt_->K_;double spot = GET(mod_->spot_,0);
    double delta = 0.0;
    double df = 0.0;
    double exercice=0.0;
    for(int idx_instant = 1; idx_instant <= nb_timesteps_; idx_instant++){
        df = compute_DF(0, idx_instant);
        for(int idx_path = 0; idx_path < nb_paths_; idx_path++){
            exercice=is_bigger(MGET(exercises_matrix, idx_path, idx_instant) ,0);
            price += MGET(exercises_matrix, idx_path, idx_instant) * df;
            delta += (MGET(exercises_matrix, idx_path, idx_instant) - strike) * exercice * df ;
        }
    }
    cout << "price       : " << price/nb_paths_ << endl;
    cout << "Delta à t=0 : " << delta/(nb_paths_*spot) << endl;

}

