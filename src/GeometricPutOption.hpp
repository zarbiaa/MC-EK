//
// Created by amine on 4/21/2021.
//

#ifndef MC_EK_GEOMETRICPUTOPTION_HPP
#define MC_EK_GEOMETRICPUTOPTION_HPP

#include "Option.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

class GeometricPutOption: public Option{
public:
    int size_;
    GeometricPutOption(double K, double T){
        K_ = K;
        maturity_ = T;
        size_ = 2;
    }
    ~GeometricPutOption(){};
    void payoff(PnlVect* payoffs, PnlMat* stock_values);
    void compute_indic_ATM(PnlMat* stock_values, PnlMat* indic_stock_values, PnlMat* vect_strike);


};


#endif //MC_EK_GEOMETRICPUTOPTION_HPP
