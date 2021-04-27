//
// Created by amine on 4/27/2021.
//

#include "pnl/pnl_finance.h"
#include "pnl/pnl_vector.h"
#include <cmath>
using namespace std;


// price european geometric put of payoff = max(0, K - sqrt(X_1*X_2))
// at maturity
void price_european_geometric_put(PnlVect* spots, PnlVect* sigmas, double rho, double strike, double T, double r, double* ptprice, double* ptdelta){
    double sigma_1;
    double sigma_2;
    double spot_1;
    double spot_2;
    double sigma_star;
    double spot;
    double divid_rate;

    spot_1 = GET(spots, 0);
    spot_2 = GET(spots, 1);
    spot = sqrt(spot_1 * spot_2);


    sigma_1 = GET(sigmas, 0);
    sigma_2 = GET(sigmas, 1);
    sigma_star = 0.5 * sqrt(pow(sigma_1, 2) + pow(sigma_2, 2) + 2*sigma_1*sigma_2*rho);

    divid_rate = 0.125 * (pow(sigma_1, 2) + pow(sigma_2, 2) - 2*sigma_1*sigma_2*rho);

    // call the pnl library vanilla put option
    pnl_cf_put_bs(spot, strike, T, r, divid_rate, sigma_star, ptprice, ptdelta);
}
