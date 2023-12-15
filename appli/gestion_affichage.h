/*
 * gestion_affichage.h
 *
 *  Created on: 15 nov. 2023
 *      Author: maind
 */

#ifndef GESTION_AFFICHAGE_H_
#define GESTION_AFFICHAGE_H_

#include "stdbool.h"

#define DIMX 320
#define DIMY 240

typedef struct{
	char *nom;
	bool etat;
} Bouton;

typedef struct{
	Bouton tableau[10];
	int16_t indiceSommet;
} Boutons;

uint16_t centrer_x(char* texte, FontDef_t *police, uint16_t x0, uint16_t x1);
uint16_t centrer_y(char* texte, FontDef_t *police, uint16_t y0, uint16_t y1);
void init_boutons();
void effacer_boutons();
bool sameButton(Bouton bouton1, Bouton bouton2);
void bouton_actif(Bouton bouton);
void empile(char *nom);
void afficher_bouton();

#endif /* GESTION_AFFICHAGE_H_ */
