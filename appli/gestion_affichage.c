/*
 * gestion_affichage.c
 *
 *  Created on: 16 nov. 2023
 *      Author: maind
 */

#include "string.h"
#include "stdint.h"
#include "stm32f1_fonts.h"
#include "gestion_affichage.h"

uint16_t centrer_x(char *texte, FontDef_t *police){
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
	uint16_t x=(320-taille*hauteur)/2;
	return x;
}

uint16_t centrer_y(char *texte, FontDef_t *police){
	uint16_t largeur;
		if(police==&Font_7x10){
			largeur=10;
		}
		else if(police==&Font_11x18){
			largeur=18;
		}
		else if(police==&Font_16x26){
			largeur=26;
		}
	uint16_t taille=strlen(texte);
	uint16_t y=(240-largeur)/2;
	return y;
}
