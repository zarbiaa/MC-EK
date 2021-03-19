//
// Created by 33758 on 3/19/2021.
//
#include <iostream>
#include <string>
#include <ctime>
#include "../src/PolyReg.h"
#include "pnl/pnl_vector.h"

#include <string>


using namespace std;

int main(){
    string poly_type = "Chebyshev1";
    int max_degree = 2;

    PolyReg *polyreg = new PolyReg(poly_type, max_degree);
    double stock[5] = {1.08, 1.07, 0.97, 0.77, 0.84};
    double df = 1/(1+0.06);
    double CF[5] = {0, 0.07*df, 0.18*df, 0.20*df, 0.09*df};
    PnlVect *stock_values = pnl_vect_create_from_ptr(5, &stock[0]);
    PnlVect* discounted_CF = pnl_vect_create_from_ptr(5, &CF[0]);

    polyreg->fit(stock_values, discounted_CF);
    pnl_vect_print(polyreg->coeffs_);
}

