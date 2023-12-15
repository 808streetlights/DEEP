/*
 * gestion_affichage.c
 *
 *  Created on: 16 nov. 2023
 *      Author: maind
 */

#include "string.h"
#include "stdint.h"
#include "stm32f1_fonts.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "gestion_affichage.h"

uint16_t getTaille(char *texte, FontDef_t *police){
	uint16_t hauteur;
		if(police==&Font_7x10){
			hauteur=7;
		}
		else if(police==&Font_11x18){
			hauteur=11;
		}
		else if(police==&
			Font_16x26){
			hauteur=16;
		}
	uint16_t taille=strlen(texte);
	return taille*hauteur;
}

uint16_t centrer_x(char *texte, FontDef_t *police, uint16_t x0, uint16_t x1){
	uint16_t largeur;
	if(police==&Font_7x10){
		largeur=7;
	}
	else if(police==&Font_11x18){
		largeur=11;
	}
	else if(police==&
		Font_16x26){
		largeur=16;
	}
	uint16_t taille=strlen(texte);
	uint16_t x=((x1-x0)-taille*largeur)/2;
	return x;
}

uint16_t centrer_y(char *texte, FontDef_t *police, uint16_t y0, uint16_t y1){
	uint16_t hauteur;
		if(police==&Font_7x10){
			hauteur=10;
		}
		else if(police==&Font_11x18){
			hauteur=18;
		}
		else if(police==&Font_16x26){
			hauteur=26;
		}
	uint16_t taille=strlen(texte);
	uint16_t y=((y1-y0)-hauteur)/2;
	return y;
}

Boutons boutons;

void init_boutons(){
	boutons.tableau[0].etat=true;
}

void effacer_boutons(){
	boutons.indiceSommet=-1;
}

bool sameButton(Bouton bouton1, Bouton bouton2){
	return (bouton1.nom==bouton2.nom && bouton1.etat==bouton2.etat) ? true : false;
}

void bouton_actif(Bouton bouton){
	for(int i=0;i<sizeof(boutons);i++){
		if(sameButton(boutons.tableau[i],bouton)){
			boutons.tableau[i].etat=true;
		}
		else{
			boutons.tableau[i].etat=false;
		}
	}
}

void empile(char *nom){
	boutons.indiceSommet++;
	boutons.tableau[boutons.indiceSommet].nom=nom;
	boutons.tableau[boutons.indiceSommet].etat=false;
}

void afficher_boutons(){
	uint16_t nbBoutons=sizeof(boutons);
	uint16_t intervalle=DIMY/(nbBoutons+2);
	for(int i=0;i<nbBoutons;i++){

		uint16_t taille=getTaille(boutons.tableau[i].nom,&Font_11x18);

		uint16_t x0=centrer_x(boutons.tableau[i].nom,&Font_11x18,0,DIMX);
		uint16_t y0=(i+2)*intervalle;
		uint16_t x1=x0+taille;
		uint16_t y1=y0+6+18+6;

		ILI9341_DrawFilledRectangle(x0,y0,x1,y1,ILI9341_COLOR_WHITE);

		if(boutons.tableau[i].etat==true){
			int16_t x=x0-20;
			int16_t y=(y1-y0)/2;
			int16_t r=10;
			ILI9341_DrawCircle(x,y,r,ILI9341_COLOR_WHITE);
		}
	}
}
