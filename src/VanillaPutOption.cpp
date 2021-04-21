//
// Created by amine on 4/20/2021.
//

#include "VanillaPutOption.hpp"
#include "pnl/pnl_matrix.h"
/**
 * Put payoff function, stock_values is a matrix of dim (n_paths, size)
 * @param payoffs
 * @param stock_values
 */

double static fct_positive_part_put(double x){
    return (x> 0) ? x: 0;
}
double static fct_atm_put(double x, double strike) {
    return (x < strike) ? 1 : 0;
}
void VanillaPutOption::payoff(PnlVect *payoffs, PnlMat* stock_values) {
    pnl_mat_get_col(payoffs, stock_values, size_ - 1);
    pnl_vect_mult_scalar(payoffs, -1);
    pnl_vect_plus_scalar(payoffs, K_);
    pnl_vect_map_inplace(payoffs, fct_positive_part_put);
}
void VanillaPutOption::compute_indic_ATM(PnlMat* stock_values, PnlMat* indic_stock_values, PnlMat* vect_strike){
    pnl_mat_map_mat(indic_stock_values, stock_values, vect_strike, fct_atm_put);
}
