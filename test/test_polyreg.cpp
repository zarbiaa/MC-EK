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
    string poly_type = "Laguerre";
    int max_degree = 2;

    PolyReg *polyreg = new PolyReg(poly_type, max_degree);
    double stock[6] = {0.0, 1.08, 1.07, 0.97, 0.77, 0.84};
    double df = 1/(1+0.06);
    double CF[6] = {1.0 ,0, 0.07*df, 0.18*df, 0.20*df, 0.09*df};
    PnlVect *stock_values = pnl_vect_create_from_ptr(6, &stock[0]);
    PnlVect* discounted_CF = pnl_vect_create_from_ptr(6, &CF[0]);
    PnlVect* fitted_values = pnl_vect_create_from_scalar(6, 0.0);
    polyreg->fit(stock_values, discounted_CF);
    cout << "coefficients of the regression:" << endl;
    pnl_vect_print(polyreg->coeffs_);
    cout << "fitted values of discounted CF" << endl;
    polyreg->apply_fit(stock_values, fitted_values);
    pnl_vect_print(fitted_values);
    cout << "values of discounted CF:" << endl;
    pnl_vect_print(discounted_CF);
}