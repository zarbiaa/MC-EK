//
// Created by 33758 on 3/19/2021.
//

#include "PolyReg.h"
#include <iostream>

using namespace std;
using namespace alglib;

PolyReg::~PolyReg() {
    pnl_vect_free(&coeffs_);
}

enum type {
    Chebyshev1 ,
    Chebyshev2,
    Hermite,
    Legendre,
    Laguerre,
};

type choix_type(std::string input) {
    if( input == "Chebyshev1" ) return Chebyshev1;
    if( input == "Chebyshev2" ) return Chebyshev2;
    if( input == "Hermite" ) return Hermite;
    if( input == "Legendre" ) return Legendre;
    if( input == "Laguerre" ) return Laguerre;
}

void PolyReg::fit(PnlVect *stock_values, PnlVect *discounted_CF) {
    int n_obs = stock_values->size;
    double xy[n_obs][max_degree_ + 2];

    switch( choix_type(poly_type_) )
    {
        case Chebyshev1: {
            for(int i = 0; i < n_obs; i++){
                for(int degree = 0; degree <= max_degree_; degree++){
                    xy[i][degree] = chebyshevcalculate(1, degree, GET(stock_values, i));
                }
                xy[i][max_degree_ + 1] = GET(discounted_CF, i);
                //cout<<xy[i][0]<<" | "<<xy[i][1]<<" | "<<xy[i][2]<<" | "<<xy[i][3]<<endl;
            }
            break;
        }
        case Chebyshev2: {
            for(int i = 0; i < n_obs; i++){
                for(int degree = 0; degree <= max_degree_; degree++){
                    xy[i][degree] = chebyshevcalculate(2, degree, GET(stock_values, i));
                }
                xy[i][max_degree_ + 1] = GET(discounted_CF, i);
                //cout<<xy[i][0]<<" | "<<xy[i][1]<<" | "<<xy[i][2]<<" | "<<xy[i][3]<<endl;
            }
            break;
        }
        case Hermite: {
            for(int i = 0; i < n_obs; i++){
                for(int degree = 0; degree <= max_degree_; degree++){
                    xy[i][degree] = hermitecalculate( degree, GET(stock_values, i));
                }
                xy[i][max_degree_ + 1] = GET(discounted_CF, i);
                //cout<<xy[i][0]<<" | "<<xy[i][1]<<" | "<<xy[i][2]<<" | "<<xy[i][3]<<endl;
            }
            break;
        }
        case Legendre: {
            for(int i = 0; i < n_obs; i++){
                for(int degree = 0; degree <= max_degree_; degree++){
                    xy[i][degree] = legendrecalculate(degree, GET(stock_values, i));
                }
                xy[i][max_degree_ + 1] = GET(discounted_CF, i);
                //cout<<xy[i][0]<<" | "<<xy[i][1]<<" | "<<xy[i][2]<<" | "<<xy[i][3]<<endl;
            }
            break;
        }
        case Laguerre: {
            for(int i = 0; i < n_obs; i++){
                for(int degree = 0; degree <= max_degree_; degree++){
                    xy[i][degree] = laguerrecalculate(degree, GET(stock_values, i));
                }
                xy[i][max_degree_ + 1] = GET(discounted_CF, i);
                //cout<<xy[i][0]<<" | "<<xy[i][1]<<" | "<<xy[i][2]<<" | "<<xy[i][3]<<endl;
            }
            break;
        }
    }

    real_2d_array xy_2d_array;
    xy_2d_array.attach_to_ptr(n_obs,max_degree_+2,&xy[0][0]);
    //printf("%s\n", xy_2d_array.tostring(4).c_str());


    ae_int_t info;
    ae_int_t nvars;
    linearmodel model;
    lrreport rep;
    real_1d_array c;

    lrbuildz(xy_2d_array, n_obs, max_degree_ + 1, info, model, rep);
    //printf("%d\n", int(info)); // EXPECTED: 1
    lrunpack(model, c, nvars);
    //printf("%s\n", c.tostring(4).c_str()); // EXPECTED: [1.98650,0.00000]
    coeffs_ = pnl_vect_create_from_ptr(max_degree_ + 1, &c[0]);
}
void PolyReg::apply_fit(PnlVect* stock_values, PnlVect* fitted_values){
    int n_obs = stock_values->size;
    double fitted_value = 0;
    double stock_value = 0;
    for(int i = 0; i < n_obs; i++){
        fitted_value = 0;
        stock_value = GET(stock_values, i);
        //cout << "stock value at " << i << "= " << stock_value << endl;
        switch( choix_type(poly_type_) ) {
            case Chebyshev1: {
                for (int degree = 0; degree <= max_degree_; degree++) {
                    fitted_value += GET(coeffs_, degree) * chebyshevcalculate(1, degree, stock_value);
                    //cout << "fitted value at " << i << "= " << fitted_value << endl;
                }
                break;
            }
            case Chebyshev2: {
                for (int degree = 0; degree <= max_degree_; degree++) {
                    fitted_value += GET(coeffs_, degree) * chebyshevcalculate(2, degree, stock_value);
                    //cout << "fitted value at " << i << "= " << fitted_value << endl;
                }
                break;
            }
            case Hermite: {
                for (int degree = 0; degree <= max_degree_; degree++) {
                    fitted_value += GET(coeffs_, degree) * hermitecalculate(degree, stock_value);
                    //cout << "fitted value at " << i << "= " << fitted_value << endl;
                }
                break;
            }
            case Legendre: {
                for (int degree = 0; degree <= max_degree_; degree++) {
                    fitted_value += GET(coeffs_, degree) * legendrecalculate(degree, stock_value);
                    //cout << "fitted value at " << i << "= " << fitted_value << endl;
                }
                break;
            }
            case Laguerre: {
                for (int degree = 0; degree <= max_degree_; degree++) {
                    fitted_value += GET(coeffs_, degree) * laguerrecalculate(degree, stock_value);
                    //cout << "fitted value at " << i << "= " << fitted_value << endl;
                }
                break;
            }
        }
        LET(fitted_values, i) = fitted_value;
    }
}