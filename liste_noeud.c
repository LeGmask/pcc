#define _GNU_SOURCE

#include "liste_noeud.h"
#include <stdlib.h>
#include <math.h>

struct _cellule {
    noeud_id_t noeud;
    double distance;
    noeud_id_t precedent;
    struct _cellule *suivant;
};

typedef struct _cellule cellule_t;

struct liste_noeud_t {
    cellule_t *tete;
    cellule_t *queue;
};

liste_noeud_t *creer_liste() {
    liste_noeud_t *liste = malloc(sizeof(liste_noeud_t));
    liste->tete = NULL;
    liste->queue = NULL;
    return liste;
}

void detruire_liste(liste_noeud_t **liste_ptr) {
    cellule_t *curseur = (*liste_ptr)->tete;
    while (curseur != NULL) {
        cellule_t *tmp = curseur;
        curseur = curseur->suivant;
        free(tmp);
    }
    free(*liste_ptr);
    *liste_ptr = NULL;
}

bool est_vide_liste(const liste_noeud_t *liste) {
    return liste->tete == NULL;
}

bool contient_noeud_liste(const liste_noeud_t *liste, noeud_id_t noeud) {
    cellule_t *curseur = liste->tete;

    while (curseur != NULL && curseur->noeud != noeud) {
        curseur = curseur->suivant;
    }

    return curseur != NULL;
}

bool contient_arrete_liste(const liste_noeud_t *liste, noeud_id_t source, noeud_id_t destination) {
    cellule_t *curseur = liste->tete;

    while (curseur != NULL && (curseur->noeud != destination || curseur->precedent != source)) {
        curseur = curseur->suivant;
    }

    return curseur != NULL;
}

double distance_noeud_liste(const liste_noeud_t *liste, noeud_id_t noeud) {
    cellule_t *curseur = liste->tete;

    while (curseur != NULL && curseur->noeud != noeud) {
        curseur = curseur->suivant;
    }

    return curseur == NULL ? INFINITY : curseur->distance;
}

noeud_id_t precedent_noeud_liste(const liste_noeud_t *liste, noeud_id_t noeud) {
    cellule_t *curseur = liste->tete;

    while (curseur != NULL && curseur->noeud != noeud) {
        curseur = curseur->suivant;
    }

    return curseur == NULL ? NO_ID : curseur->precedent;
}

noeud_id_t min_noeud_liste(const liste_noeud_t *liste) {
    cellule_t *curseur = liste->tete;

    noeud_id_t id_min = NO_ID;
    double dist_min = INFINITY;

    while (curseur != NULL) { // parcours de toute la liste chainé
        if (curseur->distance < dist_min) {
            id_min = curseur->noeud;
            dist_min = curseur->distance;
        }
        curseur = curseur->suivant;
    }

    return id_min;
}

void inserer_noeud_liste(liste_noeud_t* liste, noeud_id_t noeud, noeud_id_t precedent, double distance) {
    cellule_t *nouvelle_cellule = (cellule_t* )malloc(sizeof(cellule_t));
    nouvelle_cellule->noeud = noeud;
    nouvelle_cellule->precedent = precedent;
    nouvelle_cellule->distance = distance;
    nouvelle_cellule->suivant = NULL;

    if (liste->queue == NULL) {
        liste->tete = nouvelle_cellule;
        liste->queue = nouvelle_cellule;
    } else {
        liste->queue->suivant = nouvelle_cellule;
        liste->queue = nouvelle_cellule;
    }

}


void changer_noeud_liste(liste_noeud_t *liste, noeud_id_t noeud, noeud_id_t precedent, double distance) {
    cellule_t *curseur = liste->tete;

    while (curseur != NULL && curseur->noeud != noeud) {
        curseur = curseur->suivant;
    }

    if (curseur != NULL) {
        curseur->distance = distance;
        curseur->precedent = precedent;
    }
    else {
        inserer_noeud_liste(liste, noeud, precedent, distance);
    }
}

void supprimer_noeud_liste(liste_noeud_t* liste, noeud_id_t noeud) {
    cellule_t *curseur = liste->tete;
    cellule_t *precedent = NULL;

    while (curseur != NULL && curseur->noeud != noeud) {
        precedent = curseur;
        curseur = curseur->suivant;
    }

    if (curseur != NULL) {
        if (precedent == NULL) {
            liste->tete = curseur->suivant;
        } else {
            precedent->suivant = curseur->suivant;
        }

        if (curseur == liste->queue) {
            liste->queue = precedent;
        }

        free(curseur);
    }
}