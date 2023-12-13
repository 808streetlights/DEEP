/*
 * jeu.h
 *
 *  Created on: 16 nov. 2023
 *      Author: maind
 */

#ifndef JEU_BASE_H_
#define JEU_BASE_H_

#include "stdbool.h"

typedef struct{
    uint16_t x;
    uint16_t y;
} Point;

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t vitesse;
	uint16_t longueur;
	uint16_t largeur;
	uint16_t couleur;
} Plateforme;

typedef struct{
	int16_t x;
	int16_t y;
	int16_t r;
	uint16_t vitesse;
	int16_t vitesse_x;
	int16_t vitesse_y;
	uint16_t couleur;
} Boule;

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t cote;
	uint16_t couleur;
	bool etat;
} Brique;

void effacer_ecran(uint16_t couleur);

void afficher_plateforme(Plateforme plateforme);
void init_plateforme(Plateforme *plateforme);
void deplacer_gauche_plateforme(Plateforme *plateforme, uint16_t couleur);
void deplacer_droite_plateforme(Plateforme *plateforme, uint16_t couleur);

void init_boule(Boule *boule);
void afficher_boule(Boule boule);
void deplacement_boule(Plateforme plateforme, Boule *boule, uint16_t couleur);
void rebond_boule_horizontal(Boule *boule);
void rebond_boule_vertical(Boule *boule);
void determine_vitesse_y(Boule *boule);
bool detecte_collision_plateforme(Plateforme plateforme, Boule boule);
Point point_collision_plateforme(Plateforme plateforme, Boule boule);
void collision_plateforme_boule(Plateforme plateforme, Boule *boule);
void init_brique();
void afficher_brique();
bool detecte_collision_brique(Brique brique, Boule boule);
void effacer_brique(Brique brique);
void collision_brique_boule(Boule *boule);

#endif /* JEU_BASE_H_ */
