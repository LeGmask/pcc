#include "dijkstra.h"
#include <stdlib.h>
#include <math.h>

/**
 * construire_chemin_vers - Construit le chemin depuis le noeud de départ donné vers le
 * noeud donné. On passe un chemin en entrée-sortie de la fonction, qui est mis à jour
 * par celle-ci.
 *
 * Le noeud de départ est caractérisé par un prédécesseur qui vaut `NO_ID`.
 *
 * Ce sous-programme fonctionne récursivement :
 *  1. Si le noeud a pour précédent `NO_ID`, on a fini (c'est le noeud de départ, le chemin de
 *     départ à départ se compose du simple noeud départ)
 *  2. Sinon, on construit le chemin du départ au noeud précédent (appel récursif)
 *  3. Dans tous les cas, on ajoute le noeud au chemin, avec les caractéristiques associées dans visites
 *
 * @param chemin [in/out] chemin dans lequel enregistrer les étapes depuis le départ vers noeud
 * @param visites [in] liste des noeuds visités créée par l'algorithme de Dijkstra
 * @param noeud noeud vers lequel on veut construire le chemin depuis le départ
 */
void construire_chemin_vers(liste_noeud_t** chemin, liste_noeud_t* visites, noeud_id_t noeud) {
    noeud_id_t np = precedent_noeud_liste(visites, noeud);
    if (np != NO_ID) {
        construire_chemin_vers(chemin, visites, np);
        inserer_noeud_liste( *chemin, np, precedent_noeud_liste(visites, np), distance_noeud_liste(visites, np));
    }
}


float dijkstra(
    const struct graphe_t* graphe, 
    noeud_id_t source, noeud_id_t destination, 
    liste_noeud_t** chemin) {

    liste_noeud_t* aVisiter = creer_liste();
    liste_noeud_t* visites = creer_liste();

    inserer_noeud_liste(aVisiter, source, NO_ID, 0);
    while (!est_vide_liste(aVisiter)) {
        noeud_id_t nc = min_noeud_liste(aVisiter);
        inserer_noeud_liste(visites, nc, precedent_noeud_liste(aVisiter, nc), distance_noeud_liste(aVisiter, nc));
        supprimer_noeud_liste(aVisiter, nc);

        size_t nb_voisins = nombre_voisins(graphe, nc);
        noeud_id_t* voisins = (noeud_id_t* ) malloc(nb_voisins);
        noeuds_voisins(graphe, nc, voisins);
        for (size_t i =0; i < nb_voisins; i++ ) {
            noeud_id_t nv = voisins[i];
            if (!contient_noeud_liste(visites, nv)) {
                double d_actu = distance_noeud_liste(visites, nc) + noeud_distance(graphe, nc, nv);

                if (distance_noeud_liste(aVisiter, nv) > d_actu) {
                    changer_noeud_liste(aVisiter, nv, nc, d_actu);
                }
            }
        }

        free(voisins);
    }

    if (chemin != NULL) {

        *chemin = creer_liste();
        inserer_noeud_liste(*chemin, destination, precedent_noeud_liste(visites, destination),
                            distance_noeud_liste(visites, destination));

        construire_chemin_vers(chemin, visites, destination);

    }

    float res = (float)  distance_noeud_liste(visites, destination);

    detruire_liste(&aVisiter);
    detruire_liste(&visites);

    return res;
}



