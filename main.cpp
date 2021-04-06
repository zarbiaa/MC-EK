#include <iostream>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "alglib-3.17.0.cpp.gpl/cpp/src/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alglib-3.17.0.cpp.gpl/cpp/src/dataanalysis.h"
using namespace alglib;


int main() {
    //PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //pnl_rng_sseed(rng, time(NULL));
    int K=110;
    real_2d_array paths = "[[1,1.09,1.08,1.34],[1,1.16,1.26,1.54],[1,1.22,1.07,1.03],[1,0.93,0.97,0.92],[1,1.11,1.56,1.52],[1,0.76,0.77,0.90],[1,0.92,0.84,1.01],[1,0.88,1.22,1.34]]";
    real_2d_array xy2= "[[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0]]";
    int i;
    for (i=0;i<8;i++)
    {


    }

    return 0;
}
