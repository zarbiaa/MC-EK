//
// Created by amine on 3/16/2021.
//

#ifndef MC_EK_OPTION_H
#define MC_EK_OPTION_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


class Option {
public:
    double maturity_;
    double K_; // strike of the option, vanilla or geometric put
    virtual void payoff(PnlVect *payoffs, PnlMat* stock_values) = 0;
    virtual void compute_indic_ATM(PnlMat* stock_values, PnlMat* indic_stock_values, PnlMat* vect_strike) = 0;
};


#endif //MC_EK_OPTION_H
