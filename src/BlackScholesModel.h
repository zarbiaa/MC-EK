//
// Created by amine on 3/16/2021.
//

#ifndef MC_EK_BLACKSCHOLESMODEL_H
#define MC_EK_BLACKSCHOLESMODEL_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
class BlackScholesModel {
public:
    int size_; // nombre d'actifs dans le modèle
    double r_; // taux sans risque
    double rho_; // paramètre de corrélation
    PnlVect* sigma_; // vecteur de volatilités
    PnlVect* spot_; // valeurs spot des actifs

    PnlMat *correlatioMatrix; // matrice de corrélation des actifs
    PnlMat* G; // matrice de vecteur gaussiens
    PnlVect* Ld; // vecteur de la matrice triangulaire (extraite par Cholesky)
    PnlVect *Gi; // Vecteur gaussien 'i'

    /*
     * Destructeur de l'objet BlackScholesModel, fait appel aux méthodes
     * free de la librarie PNL
     * */
    ~BlackScholesModel();
    /*
     * Constructeur de l'objet BlackScholesModel
     */

    BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect *spot, int nbTimeSteps);
    /*
     * Première méthode: génère une trajectoire du modèle et la stocke dans path
     */
    void generate_path(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng);

};


#endif //MC_EK_BLACKSCHOLESMODEL_H
