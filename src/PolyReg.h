//
// Created by 33758 on 3/19/2021.
//
#include <iostream>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

#include "../alglib-3.17.0.cpp.gpl/cpp/src/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../alglib-3.17.0.cpp.gpl/cpp/src/dataanalysis.h"
#include "../alglib-3.17.0.cpp.gpl/cpp/src/specialfunctions.h"
#include <string>

using namespace std;
using namespace alglib;

#ifndef MC_EK_POLYREG_H
#define MC_EK_POLYREG_H


class PolyReg {
public:
    string poly_type_;
    int max_degree_;
    PnlVect * coeffs_;

    PolyReg(string poly_type, int max_degree){
        poly_type_ = poly_type;
        max_degree_ = max_degree;
        coeffs_ = pnl_vect_create(max_degree_);
    }

    ~PolyReg();

    void fit(PnlVect* stock_values, PnlVect* discounted_CF);

    void apply_fit(PnlVect* stock_values, PnlVect* fitted_values);
};


#endif //MC_EK_POLYREG_H
