//
// Created by 33758 on 3/18/2021.
//
#include <iostream>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

#include "pnl/pnl_random.h"
#include "alglib-3.17.0.cpp.gpl/cpp/src/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alglib-3.17.0.cpp.gpl/cpp/src/dataanalysis.h"
#include "alglib-3.17.0.cpp.gpl/cpp/src/specialfunctions.h"

using namespace std;
using namespace alglib;

/*double fit(real_1d_array parametres[],double var[])
{
   double y;
   int i;
   for (i=0;i++)
   {
       if (parametres[i]==0)
       {break;}
       y+=parametres[i]*var[i];
   }
   return y;

}
*/

int main() {
    int r=0.06;
    int K=1.10;
    //real_2d_array paths = "[[1,1.09,1.08,1.34],[1,1.16,1.26,1.54],[1,1.22,1.07,1.03],[1,0.93,0.97,0.92],[1,1.11,1.56,1.52],[1,0.76,0.77,0.90],[1,0.92,0.84,1.01],[1,0.88,1.22,1.34]]";
    double paths[8][4] = {{1,1.09,1.08,1.34},{1,1.16,1.26,1.54},{1,1.22,1.07,1.03},{1,0.93,0.97,0.92},{1,1.11,1.56,1.52},{1,0.76,0.77,0.90},{1,0.92,0.84,1.01},{1,0.88,1.22,1.34}};
    real_2d_array xy2;
    double xy22[100][100];
    double reg_xy22[5][4];
    int i,j=0;
    double x,y;
    for (i=0;i<8;i++)
    {
        if (paths[i][2]<1.1)
        {
            y=1.1-paths[i][3];
            x=max(y,0.0);
            xy22[j][0]=x/(1+0.06);
            xy22[j][1]=paths[i][2];
            xy22[j][2]=i;
            reg_xy22[j][0]=alglib::chebyshevcalculate(2, 0, paths[i][2]);
            reg_xy22[j][1]=alglib::chebyshevcalculate(2, 1, paths[i][2]);
            reg_xy22[j][2]=alglib::chebyshevcalculate(2, 2, paths[i][2]);
            //reg_xy22[j][3]=alglib::chebyshevcalculate(2, 3, paths[i][2]);
            //reg_xy22[j][4]=alglib::chebyshevcalculate(2, 4, paths[i][2]);
            reg_xy22[j][3]=x/(1+0.06);
            //cout<<reg_xy22[j][0]<<" | "<<reg_xy22[j][1]<<" | "<<reg_xy22[j][2]<<" | "<<reg_xy22[j][3]<<" | "<<reg_xy22[j][4]<<" | "<<reg_xy22[j][5]<<endl;
            j+=1;
        }
    }
    xy2.attach_to_ptr(5,4,&reg_xy22[0][0]);
    printf("%s\n", xy2.tostring(4).c_str());
    ae_int_t info;
    ae_int_t nvars;
    linearmodel model;
    lrreport rep;
    real_1d_array c;

    lrbuildz(xy2, 5, 3, info, model, rep);
    printf("%d\n", int(info)); // EXPECTED: 1
    lrunpack(model, c, nvars);
    printf("%s\n", c.tostring(4).c_str()); // EXPECTED: [1.98650,0.00000]
    /*double xy[5][2];
    for (i=0;i<5;i++)
    {
      xy[i][0]=reg_xy22[i][3];
      xy[i][1]=fit(c,reg_xy22[i]);
      cout<<xy[i][0]<<"|"<<xy[i][1]<<endl;
    }
     */
    PnlVect* coeffs = pnl_vect_create_from_ptr(4, &c[0]);
    pnl_vect_print(coeffs);

    PnlVect* z = pnl_vect_create_from_ptr(4,reg_xy22[1]);
    pnl_vect_print(z);

    double fitted_value = pnl_vect_scalar_prod(coeffs, z);
    cout << "fitted value: " << fitted_value << endl;

    return 0;
}
