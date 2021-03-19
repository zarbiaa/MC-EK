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

void PolyReg::fit(PnlVect *stock_values, PnlVect *discounted_CF) {
    int n_obs = stock_values->size;
    double xy[n_obs][max_degree_ + 2];

    for(int i = 0; i < n_obs; i++){
        for(int degree = 0; degree <= max_degree_; degree++){
            xy[i][degree] = chebyshevcalculate(2, degree, GET(stock_values, i));
        }
        xy[i][max_degree_ + 1] = GET(discounted_CF, i);
        cout<<xy[i][0]<<" | "<<xy[i][1]<<" | "<<xy[i][2]<<" | "<<xy[i][3]<<endl;


    }

    real_2d_array xy_2d_array;
    xy_2d_array.attach_to_ptr(n_obs,max_degree_+2,&xy[0][0]);
    printf("%s\n", xy_2d_array.tostring(4).c_str());


    ae_int_t info;
    ae_int_t nvars;
    linearmodel model;
    lrreport rep;
    real_1d_array c;

    lrbuildz(xy_2d_array, n_obs, max_degree_ + 1, info, model, rep);
    printf("%d\n", int(info)); // EXPECTED: 1
    lrunpack(model, c, nvars);
    printf("%s\n", c.tostring(4).c_str()); // EXPECTED: [1.98650,0.00000]
    coeffs_ = pnl_vect_create_from_ptr(max_degree_ + 1, &c[0]);
}
