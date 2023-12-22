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

#define abs(a) ((a >= 0) ? a : -a)

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

void init_boutons(){
	boutons.tableau[0].etat=true;
	boutons.indiceSommet=-1;
}

void ajouter_bouton(char *nom){
	boutons.indiceSommet++;
	boutons.tableau[boutons.indiceSommet].nom=nom;
}

uint16_t getNombreBoutons(){
	return boutons.indiceSommet+1;
}

uint16_t getIndiceBoutonActif(){
	for(int i=0;i<getNombreBoutons();i++){
		if(boutons.tableau[i].etat==true){
			return i;
		}
	}
}

void effacer_boutons(){
	boutons.indiceSommet=-1;
}

bool sameButton(Bouton bouton1, Bouton bouton2){
	return (bouton1.nom==bouton2.nom) ? true : false;
}

void activer_bouton(Bouton bouton){
	for(int i=0;i<getNombreBoutons();i++){
		if(sameButton(boutons.tableau[i],bouton)){
			boutons.tableau[i].etat=true;
		}
		else{
			boutons.tableau[i].etat=false;
		}
	}
}

void monter_bouton(){
	if((getIndiceBoutonActif()-1)>=0){
		uint16_t nouveau_bouton_actif=getIndiceBoutonActif()-1;
		activer_bouton(boutons.tableau[nouveau_bouton_actif]);
		effacer_affichage_boutons();
		afficher_boutons();
	}
}

void descendre_bouton(){
	if((getIndiceBoutonActif()+1)<=getNombreBoutons()-1){
		uint16_t nouveau_bouton_actif=getIndiceBoutonActif()+1;
		activer_bouton(boutons.tableau[nouveau_bouton_actif]);
		effacer_affichage_boutons();
		afficher_boutons();
	}
}

void afficher_boutons(){
	uint16_t intervalle=DIMY/(1+(getNombreBoutons())+1);
	for(int i=0;i<getNombreBoutons();i++){

		uint16_t taille=getTaille(boutons.tableau[i].nom,&Font_11x18);

		uint16_t x_texte=centrer_x(boutons.tableau[i].nom,&Font_11x18,0,DIMX);
		uint16_t y_texte=(i+1)*intervalle+intervalle/2;

		boutons.tableau[i].x0=x_texte-12;
		boutons.tableau[i].y0=y_texte-6;

		boutons.tableau[i].x1=x_texte+taille+12;
		boutons.tableau[i].y1=y_texte+18+6;

	if(boutons.tableau[i].etat==false){
		ILI9341_DrawRectangle(boutons.tableau[i].x0,boutons.tableau[i].y0,boutons.tableau[i].x1,boutons.tableau[i].y1,ILI9341_COLOR_WHITE);
		ILI9341_Puts(x_texte,y_texte,boutons.tableau[i].nom,&Font_11x18,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK);
		}
	else{
		ILI9341_DrawFilledRectangle(boutons.tableau[i].x0,boutons.tableau[i].y0,boutons.tableau[i].x1,boutons.tableau[i].y1,ILI9341_COLOR_WHITE);
		ILI9341_Puts(x_texte,y_texte,boutons.tableau[i].nom,&Font_11x18,ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
		}
	}
}

void effacer_affichage_boutons(){
	for(int i=0;i<getNombreBoutons();i++){
		ILI9341_DrawFilledRectangle(boutons.tableau[i].x0,boutons.tableau[i].y0,boutons.tableau[i].x1,boutons.tableau[i].y1,ILI9341_COLOR_BLACK);
	}
}
