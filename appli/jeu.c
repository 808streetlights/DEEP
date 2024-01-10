/*
 * jeu.c
 *
 *  Created on: 29 nov. 2023
 *      Author: maind
 */

#include "tft_ili9341/stm32f1_ili9341.h"
#include "MatrixKeyboard/matrix_keyboard.h"
#include "gestion_affichage.h"
#include "jeu_base.h"
#include "jeu.h"

#define DIMX 320
#define DIMY 240

uint16_t BGC=ILI9341_COLOR_BLACK;

void ecran_titre(){
	Plateforme plateformeB;
		plateformeB.x=20;
		plateformeB.y=225;
		plateformeB.vitesse=5;
		plateformeB.longueur=40;
		plateformeB.largeur=6;
		plateformeB.couleur=ILI9341_COLOR_WHITE;

	Plateforme plateformeH;
		plateformeH.x=300;
		plateformeH.y=15;
		plateformeH.vitesse=5;
		plateformeH.longueur=40;
		plateformeH.largeur=6;
		plateformeH.couleur=ILI9341_COLOR_WHITE;

	ILI9341_Puts(centrer_x("Brick Breaker", &Font_16x26, 0, 320),centrer_y("Brick Breaker", &Font_16x26, 0, 240),"Brick Breaker", &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

	if(plateformeB.x-((plateformeB.longueur)/2)==0 && plateformeH.x+((plateformeH.longueur)/2)==320){
		while(plateformeB.x+((plateformeB.longueur)/2)!=320 && plateformeH.x-((plateformeH.longueur)/2)!=0){
			deplacer_droite_plateforme(&plateformeB,BGC);
			deplacer_gauche_plateforme(&plateformeH,BGC);
			afficher_plateforme(plateformeB);
			afficher_plateforme(plateformeH);
		}
	}
	if(plateformeH.x-((plateformeH.longueur)/2)==0 && plateformeB.x+((plateformeB.longueur)/2)==320){
		while(plateformeH.x+((plateformeH.longueur)/2)!=320 && plateformeB.x-((plateformeB.longueur)/2)!=0){
			deplacer_droite_plateforme(&plateformeH,BGC);
			deplacer_gauche_plateforme(&plateformeB,BGC);
			afficher_plateforme(plateformeH);
			afficher_plateforme(plateformeB);
		}
	}
}

void initialisation_menu_principal(){
	init_boutons();
	ajouter_bouton("Play");
	ajouter_bouton("Difficulty");
	uint16_t intervalle=DIMY/(1+(boutons.indiceSommet+1)+1);
	ILI9341_Puts(centrer_x("Brick Breaker",&Font_11x18,0,DIMX),intervalle/2,"Brick Breaker",&Font_11x18,ILI9341_COLOR_WHITE,BGC);
	afficher_boutons();
}

void initialisation_difficulte(){
	init_boutons();
	ajouter_bouton("EASY");
	ajouter_bouton("NORMAL");
	ajouter_bouton("HARD");
	uint16_t intervalle=DIMY/(1+(boutons.indiceSommet+1)+1);
	ILI9341_Puts(centrer_x("Difficulty",&Font_11x18,0,DIMX),intervalle/2,"Difficulty",&Font_11x18,ILI9341_COLOR_WHITE,BGC);
	afficher_boutons();
}

void initialisation_jeu(Plateforme *plateforme, Boule *boule){
	init_plateforme(plateforme);
	init_brique();
	init_boule(boule);
	afficher_brique();
}

void deroulement_jeu(Plateforme *plateforme, Boule *boule){
	matrix_keyboard_demo_process_main();
	if(KEYBOARD_get_key()=='6'){
		deplacer_droite_plateforme(plateforme,ILI9341_COLOR_BLACK);
	}
	if(KEYBOARD_get_key()=='4'){
		deplacer_gauche_plateforme(plateforme,ILI9341_COLOR_BLACK);
	}
	afficher_plateforme(*plateforme);
	deplacement_boule(*plateforme,boule,ILI9341_COLOR_BLACK);
	afficher_boule(*boule);
	HAL_Delay(10);
}

void initialisation_pause(){
	init_boutons();
	ajouter_bouton("Resume");
	ajouter_bouton("Restart");
	ajouter_bouton("Main menu");
	uint16_t intervalle=DIMY/(1+(boutons.indiceSommet+1)+1);
	ILI9341_Puts(centrer_x("Pause",&Font_11x18,0,DIMX),intervalle/2,"Pause",&Font_11x18,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK);
	afficher_boutons();
}

void initialisation_game_over(){
	init_boutons();
	ajouter_bouton("Play again");
	ajouter_bouton("Main menu");
	uint16_t intervalle=DIMY/(1+(boutons.indiceSommet+1)+1);
	ILI9341_Puts(centrer_x("Game Over",&Font_11x18,0,DIMX),intervalle/2,"Game Over",&Font_11x18,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK);
	afficher_boutons();
}

void initialisation_win(){
	init_boutons();
	ajouter_bouton("Play again");
	ajouter_bouton("Main menu");
	uint16_t intervalle=DIMY/(1+(boutons.indiceSommet+1)+1);
	ILI9341_Puts(centrer_x("You won! Congrats ;)",&Font_11x18,0,DIMX),intervalle/2,"You won! Congrats ;)",&Font_11x18,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK);
	afficher_boutons();
}

void deplacements_menus(){
	if(KEYBOARD_get_key()=='2'){
		monter_bouton();
	}
	if(KEYBOARD_get_key()=='8'){
		descendre_bouton();
	}
}
