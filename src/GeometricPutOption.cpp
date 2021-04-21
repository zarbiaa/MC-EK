//
// Created by amine on 4/21/2021.
//

#include "GeometricPutOption.hpp"
#include <cmath>

double static fct_sqrt_product(double x, double y){
    return sqrt(x*y);
}
double static fct_positive_part_put(double x){
    return (x> 0) ? x: 0;
}
double static fct_atm_put(double x, double strike) {
    return (x < strike) ? 1 : 0;
}

void GeometricPutOption::payoff(PnlVect *payoffs, PnlMat* stock_values) {
    int nb_paths = stock_values->m;
    PnlVect *stock_values_1 = pnl_vect_create(nb_paths);
    PnlVect *stock_values_2 = pnl_vect_create(nb_paths);
    pnl_mat_get_col(stock_values_1, stock_values, 0);
    pnl_mat_get_col(stock_values_2, stock_values, 1);

    pnl_vect_map_vect(payoffs, stock_values_1, stock_values_2, fct_sqrt_product);
    pnl_vect_mult_scalar(payoffs, -1);
    pnl_vect_plus_scalar(payoffs, K_);
    pnl_vect_map_inplace(payoffs, fct_positive_part_put);
    pnl_vect_free(&stock_values_1); pnl_vect_free(&stock_values_2);
}

void GeometricPutOption::compute_indic_ATM(PnlMat* stock_values, PnlMat* indic_stock_values, PnlMat* vect_strike) {
    int nb_paths = stock_values->m;
    PnlVect *stock_values_1 = pnl_vect_create(nb_paths);
    PnlVect *stock_values_2 = pnl_vect_create(nb_paths);
    PnlVect *tmp_indic = pnl_vect_create(nb_paths);
    PnlVect* tmp_indic_2 = pnl_vect_create(nb_paths);
    PnlVect* tmp_strike = pnl_vect_create(nb_paths);
    pnl_mat_get_col(tmp_strike, vect_strike, 0);
    pnl_mat_get_col(stock_values_1, stock_values, 0);
    pnl_mat_get_col(stock_values_2, stock_values, 1);
    pnl_vect_map_vect(tmp_indic, stock_values_1, stock_values_2, fct_sqrt_product);
    pnl_vect_map_vect(tmp_indic_2, tmp_indic, tmp_strike, fct_atm_put);

    for(int idx=0; idx < size_; idx++){
        pnl_mat_set_col(indic_stock_values, tmp_indic_2, idx);
    }
    pnl_vect_free(&stock_values_1); pnl_vect_free(&stock_values_2);
    pnl_vect_free(&tmp_indic); pnl_vect_free(&tmp_indic_2); pnl_vect_free(&tmp_strike);
}
