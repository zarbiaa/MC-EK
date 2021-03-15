//
// Created by amine on 3/15/2021.
//
#include <iostream>
#include "alglib-3.17.0.cpp.gpl/cpp/src/specialfunctions.h"
#include "pnl/pnl_vector.h"

using namespace std;
#include <stdio.h>

int main() {
    cout << "hi" << endl;
    int size = 5;
    double argument = 2.68465154;
    PnlVect* vect = pnl_vect_create_from_scalar(size, argument);
    for(int i = 0; i < size; i++){
        pnl_vect_set(vect, i, alglib::chebyshevcalculate(2, 4, GET(vect, i)));
    }
    pnl_vect_print(vect);
    return 0;
}

