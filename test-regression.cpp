//
// Created by amine on 3/15/2021.
//
#include "alglib-3.17.0.cpp.gpl/cpp/src/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alglib-3.17.0.cpp.gpl/cpp/src/dataanalysis.h"

using namespace alglib;


int main(int argc, char **argv)
{
    //
    // In this example we demonstrate linear fitting by f(x|a) = a*exp(0.5*x).
    //
    // We have:
    // * xy - matrix of basic function values (exp(0.5*x)) and expected values
    //
    real_2d_array xy = "[[0.606531,1.133719],[0.670320,1.306522],[0.740818,1.504604],[0.818731,1.554663],[0.904837,1.884638],[1.000000,2.072436],[1.105171,2.257285],[1.221403,2.534068],[1.349859,2.622017],[1.491825,2.897713],[1.648721,3.219371]]";
    ae_int_t info;
    ae_int_t nvars;
    linearmodel model;
    lrreport rep;
    real_1d_array c;

    lrbuildz(xy, 11, 1, info, model, rep);
    printf("%d\n", int(info)); // EXPECTED: 1
    lrunpack(model, c, nvars);
    printf("%s\n", c.tostring(4).c_str()); // EXPECTED: [1.98650,0.00000]
    return 0;
}

