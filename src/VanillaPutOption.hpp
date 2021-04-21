//
// Created by amine on 4/20/2021.
//

#ifndef MC_EK_VANILLAPUTOPTION_HPP
#define MC_EK_VANILLAPUTOPTION_HPP

#include "Option.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

class VanillaPutOption: public Option {
public:
    int size_;
    /**
    * Constructeur de l'option put américain
     */
     VanillaPutOption(double K, double T){
         K_ = K;
         maturity_ = T;
         size_ = 1;
     }
     ~VanillaPutOption(){};

    void payoff(PnlVect* payoffs, PnlMat* stock_values);
    void compute_indic_ATM(PnlMat* stock_values, PnlMat* indic_stock_values, PnlMat* vect_strike);
};


#endif //MC_EK_VANILLAPUTOPTION_HPP
