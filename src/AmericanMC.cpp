//
// Created by amine on 4/6/2021.
//
#include <iostream>
#include <string>
#include <ctime>
#include "PolyReg.h"
#include "pnl/pnl_random.h"
#include "BlackScholesModel.h"
#include <string>
#include <cmath>


using namespace std;

void estimate_continuation(PnlVect* continuation_values,PnlVect* stock_values,PnlVect* discounted_CFs, PnlVect* regression_stocks, PnlVect* regression_CF,string poly_type, int max_degree){
    PolyReg *polyreg = new PolyReg(poly_type, max_degree);
    cout << "stock regression" << endl; pnl_vect_print(stock_values);
    cout << "continuation regression" << endl; pnl_vect_print(discounted_CFs);
    polyreg->fit(stock_values, discounted_CFs);
    cout << "regression coeffs" << endl; pnl_vect_print(polyreg->coeffs_);
    polyreg->apply_fit(stock_values, continuation_values);
}

double compute_DF(int present_instant, int future_instant, double timespan, double rf){
    return exp(-rf * (future_instant - present_instant) * timespan);
}
double positive_part_put(double x){
    return (x> 0) ? x: 0;
}
double atm_put(double x, double strike){
    return (x < strike) ? 1: 0;
}

double is_bigger(double x, double y){
    return x > y;
}
void optimal_exercise_vect(PnlVect* optimal_vect, PnlVect* payoffs, PnlVect* continuations){
    pnl_vect_map_vect(optimal_vect, payoffs, continuations, is_bigger);
}
void optimal_exercise(PnlMat* exercises_matrix, PnlVect* continuation_values, PnlVect* payoffs, PnlVect* optimal_vect, int idx_instant){
    int nb_paths = exercises_matrix->m;
    int nbTimeSteps = exercises_matrix->n - 1;
    optimal_exercise_vect(optimal_vect, payoffs, continuation_values);
    pnl_vect_mult_vect_term(payoffs, optimal_vect);
    pnl_mat_set_col(exercises_matrix, payoffs, idx_instant);

    pnl_vect_mult_scalar(optimal_vect, -1); pnl_vect_plus_scalar(optimal_vect, 1);
    PnlVect* future_CF = pnl_vect_create(nb_paths);
    for(int idx_future_instant = idx_instant + 1; idx_future_instant <= nbTimeSteps; idx_future_instant++){
        pnl_mat_get_col(future_CF, exercises_matrix, idx_future_instant);
        pnl_vect_mult_vect_term(future_CF, optimal_vect);
        pnl_mat_set_col(exercises_matrix, future_CF, idx_future_instant);
    }
    pnl_vect_free(&future_CF);

}
void payoff_put(PnlVect* stock_prices, double strike){
    pnl_vect_mult_scalar(stock_prices, -1);
    pnl_vect_plus_scalar(stock_prices, strike);
    pnl_vect_map_inplace(stock_prices, positive_part_put);
}
void ATM_stocks_indic(PnlVect* stock_prices, PnlVect* tmp_ATM_indic, PnlVect* vect_strike){
    //pnl_vect_mult_scalar(stock_prices, -1);
    //pnl_vect_plus_scalar(stock_prices, strike);
    pnl_vect_map_vect(tmp_ATM_indic, stock_prices, vect_strike, atm_put);
}
void AMC_exercises(PnlMat* exercises_matrix, PnlMat* stock_paths, double strike, double maturity, double rf, string poly_type, int max_degree){
    int nb_paths = stock_paths->m;
    int nbTimeSteps = stock_paths->n - 1;
    double timespan = maturity / (float) nbTimeSteps;
    // fill last column with last payoffs
    PnlVect *regression_stocks = pnl_vect_create(nb_paths);
    PnlVect *regression_CF = pnl_vect_create(nb_paths);
    PnlVect* vect_strike = pnl_vect_create_from_scalar(nb_paths, strike);
    PnlVect* tmp_payoffs = pnl_vect_create(nb_paths);
    PnlVect *tmp_stocks = pnl_vect_create(nb_paths);
    PnlVect *tmp_ATM_indic = pnl_vect_create(nb_paths);
    PnlVect *tmp_CFs = pnl_vect_create(nb_paths);
    PnlVect *tmp_continuation = pnl_vect_create(nb_paths);
    PnlVect *optimal_vect = pnl_vect_create(nb_paths);
    PnlVect *tmp_future_CF = pnl_vect_create(nb_paths);
    pnl_mat_get_col(tmp_payoffs, stock_paths, nbTimeSteps);
    payoff_put(tmp_payoffs, strike);
    pnl_mat_set_col(exercises_matrix, tmp_payoffs, nbTimeSteps);
    // loop on previous discretisation instants
    cout << "matrix at maturity: " << endl; pnl_mat_print(exercises_matrix);
    double DF = 0;
    for (int idx_instant = nbTimeSteps - 1; idx_instant > 0; idx_instant--){
        // retrieve ATM stock prices
        pnl_mat_get_col(tmp_stocks, stock_paths, idx_instant);
        tmp_payoffs = pnl_vect_create(nb_paths);pnl_vect_clone(tmp_payoffs, tmp_stocks);
        payoff_put(tmp_payoffs, strike);
        ATM_stocks_indic(tmp_stocks, tmp_ATM_indic, vect_strike);
        pnl_vect_mult_vect_term(tmp_stocks, tmp_ATM_indic);
        cout << "ATM stocks: " << endl; pnl_vect_print(tmp_stocks);
        // retrieve corresponding future discounted CFs
        pnl_vect_mult_scalar(tmp_CFs, 0.0);

        for (int idx_future_instant = idx_instant + 1; idx_future_instant <= nbTimeSteps; idx_future_instant++){
            DF = compute_DF(idx_instant, idx_future_instant, timespan, rf);
            pnl_mat_get_col(tmp_future_CF, exercises_matrix, idx_future_instant);
            cout << "future CF from instant "<< idx_future_instant << endl; pnl_vect_print(tmp_future_CF);

            pnl_vect_mult_scalar(tmp_future_CF, DF);
            cout << "discounted future CF from instant "<< idx_future_instant << endl; pnl_vect_print(tmp_future_CF);

            pnl_vect_plus_vect(tmp_CFs, tmp_future_CF);
            //pnl_vect_mult_vect_term(tmp_CFs, tmp_ATM_indic);
        }
        cout << "discounted future CF before indic" << endl; pnl_vect_print(tmp_CFs);

        pnl_vect_mult_vect_term(tmp_CFs, tmp_ATM_indic);
        cout << "discounted future CF" << endl; pnl_vect_print(tmp_CFs);
        estimate_continuation(tmp_continuation, tmp_stocks, tmp_CFs, regression_stocks, regression_CF, poly_type, max_degree);
        cout << "continuation values" << endl; pnl_vect_print(tmp_continuation);

        optimal_exercise(exercises_matrix, tmp_continuation, tmp_payoffs, optimal_vect, idx_instant);
        cout << "matrix at t= " << idx_instant << ": " << endl; pnl_mat_print(exercises_matrix) ; cout << endl;

    }
    pnl_vect_free(&tmp_payoffs);pnl_vect_free(&tmp_stocks);pnl_vect_free(&tmp_ATM_indic);
    pnl_vect_free(&tmp_CFs);pnl_vect_free(&tmp_continuation);pnl_vect_free(&optimal_vect);
    pnl_vect_free(&tmp_future_CF); pnl_vect_free(&vect_strike);
    pnl_vect_free(&regression_CF); pnl_vect_free(&regression_stocks);
}

int main(){
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double r = 0.06; // 0.01 %
    double spot_ = 100;
    double sigma_ = 0.3;
    int size = 2;
    PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
    double T = 1.0;
    double rho = 0;
    double strike = 1.1;
    int nbTimeSteps = 8;
    int nb_paths = 10;
    string poly_type = "Laguerre";
    int max_degree = 2;

    BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, nbTimeSteps);
    PnlMat** stock_paths = (PnlMat**) malloc(0);
    for(int i = 0; i < size; i++){
        stock_paths[i] = pnl_mat_create(nb_paths, nbTimeSteps + 1);
    }

    model->generate_paths(stock_paths, T, nb_paths, nbTimeSteps, rng);

    double stock_matrix[32] = {1.0, 1.09, 1.08, 1.34, 1.0, 1.16, 1.26, 1.54, 1, 1.22, 1.07, 1.03, 1, 0.93, 0.97, 0.92, 1, 1.11, 1.56, 1.52, 1, 0.76, 0.77, 0.90, 1, 0.92, 0.84, 1.01, 1.0, 0.88, 1.22, 1.34};
    PnlMat *stock_mat = pnl_mat_create_from_ptr(8, 4, &stock_matrix[0]);
    PnlMat * exercises_matrix = pnl_mat_create(8, 3 + 1);
    cout << "stock matrix : " << endl; pnl_mat_print(stock_mat);
    AMC_exercises(exercises_matrix, stock_mat, strike, T, r, poly_type, max_degree);
    cout << "final exercise matrix" << endl;
    pnl_mat_print(exercises_matrix);
}

