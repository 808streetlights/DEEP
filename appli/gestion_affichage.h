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
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
} Bouton;

typedef struct{
	Bouton tableau[10];
	int16_t indiceSommet;
} Boutons;

Boutons boutons;

uint16_t centrer_x(char* texte, FontDef_t *police, uint16_t x0, uint16_t x1);
uint16_t centrer_y(char* texte, FontDef_t *police, uint16_t y0, uint16_t y1);
void init_boutons();
void ajouter_bouton(char *nom);
uint16_t getNombreBoutons();
uint16_t getIndiceBoutonActif();
void effacer_boutons();
bool sameButton(Bouton bouton1, Bouton bouton2);
void activer_bouton(Bouton bouton);
void monter_bouton();
void descendre_bouton();
void afficher_boutons();
void effacer_affichage_boutons();

#endif /* GESTION_AFFICHAGE_H_ */
