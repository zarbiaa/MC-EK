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

int main() {
    string poly_type = "Chebyshev1";
    int max_degree= 2;

    /*PolyReg *polyreg = new PolyReg(poly_type, max_degree);
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
    pnl_vect_print(discounted_CF);*/

    PolyReg *polyreg = new PolyReg(poly_type, max_degree);
    double stock1[13] = {5.0, 3.0, 1.0, 8.0, 4.0, 8.0, 9.0, 3.5, 2.5, 1.2, 4.5 ,7.0,10};
    double stock2[13] = {3.0, 2.0, 5.0, 2.0, 4.0, 8.0, 9.0, 3.5, 2.5, 1.2, 4.5 ,7.0,10};
    double y[13] = {1155.0, 210.0, 165.0, 1360.0, 1296.0,18496.0,29241.0, 784.0, 225.0, 16.6464, 2025.0, 11025.0, 44100};
    PnlVect *stock_values1 = pnl_vect_create_from_ptr(13, &stock1[0]);
    PnlVect *stock_values2 = pnl_vect_create_from_ptr(13, &stock2[0]);
    PnlVect *Y = pnl_vect_create_from_ptr(13, &y[0]);
    PnlVect* fitted_values = pnl_vect_create_from_scalar(13, 0.0);
    polyreg->fit2(stock_values1, stock_values2, Y);
    cout << "coefficients of the regression:" << endl;
    pnl_vect_print(polyreg->coeffs_);
    polyreg->apply_fit2(stock_values1,stock_values2, fitted_values);
    pnl_vect_print(fitted_values);

}