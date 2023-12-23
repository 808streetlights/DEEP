/*
 * jeu.c
 *
 *  Created on: 16  nov. 2023
 *      Author: maind
 */

#include "tft_ili9341/stm32f1_ili9341.h"
#include "gestion_affichage.h"
#include "jeu_base.h"

uint16_t BGC1=ILI9341_COLOR_BLACK;

void effacer_ecran(uint16_t couleur){
	ILI9341_Fill(couleur);
}

void afficher_plateforme(Plateforme plateforme){
	uint16_t x0=plateforme.x-(plateforme.longueur)/2;
	uint16_t y0=plateforme.y-(plateforme.largeur)/2;
	uint16_t x1=plateforme.x+(plateforme.longueur)/2;
	uint16_t y1=plateforme.y+(plateforme.largeur)/2;
	ILI9341_DrawFilledRectangle(x0,y0,x1,y1,plateforme.couleur);
}

void init_plateforme(Plateforme *plateforme){
	plateforme->longueur=40;
	plateforme->largeur=6;
	plateforme->x=DIMX/2;
	plateforme->y=DIMY-15;
	plateforme->vitesse=5;
	plateforme->couleur=ILI9341_COLOR_WHITE;
}

void deplacer_gauche_plateforme(Plateforme *plateforme, uint16_t couleur){
	if(plateforme->x>(plateforme->longueur/2)){
		plateforme->x-=plateforme->vitesse;
		uint16_t x0=plateforme->x+((plateforme->longueur)/2)+plateforme->vitesse;
		uint16_t y0=plateforme->y-((plateforme->largeur)/2);
		uint16_t x1=plateforme->x+((plateforme->longueur)/2);
		uint16_t y1=plateforme->y+((plateforme->largeur)/2);
		ILI9341_DrawFilledRectangle(x0,y0,x1,y1,couleur);
	}
}

void deplacer_droite_plateforme(Plateforme *plateforme, uint16_t couleur){
    if(plateforme->x<DIMX-(plateforme->longueur/2)){
        plateforme->x+=plateforme->vitesse;
        uint16_t x0=plateforme->x-((plateforme->longueur)/2)-plateforme->vitesse;
        uint16_t y0=plateforme->y-((plateforme->largeur)/2);
		uint16_t x1=plateforme->x-((plateforme->longueur)/2);
		uint16_t y1=plateforme->y+((plateforme->largeur)/2);
        ILI9341_DrawFilledRectangle(x0,y0,x1,y1,couleur);
    }
}

uint16_t vitesse_difficulte=4;

void adapter_difficulte(uint16_t new_vitesse_difficulte){
	vitesse_difficulte=new_vitesse_difficulte;
}

void init_boule(Boule *boule){
	boule->x=DIMX/2;
	boule->y=DIMY-60;
	boule->r=4;
	boule->vitesse=vitesse_difficulte;
	boule->vitesse_x=0;
	boule->vitesse_y=1;
	boule->couleur=ILI9341_COLOR_WHITE;
}

void afficher_boule(Boule boule){
	ILI9341_DrawFilledCircle(boule.x,boule.y,boule.r,boule.couleur);
}

void deplacement_boule(Plateforme plateforme, Boule *boule, uint16_t couleur){
	ILI9341_DrawFilledCircle(boule->x,boule->y,boule->r,couleur);
	boule->x+=boule->vitesse_x;
	boule->y+=boule->vitesse_y;
	if(boule->x+boule->r+boule->vitesse_x>DIMX){
		boule->x=DIMX-boule->r;
		rebond_boule_vertical(boule);
	}
	if(boule->x-boule->r+boule->vitesse_x<0){
		boule->x=boule->r;
		rebond_boule_vertical(boule);
	}
	if(boule->y-boule->r+boule->vitesse_y<0){
		boule->y=boule->r;
		rebond_boule_horizontal(boule);
	}
	collision_brique_boule(boule);
	collision_plateforme_boule(plateforme,boule);
}

void rebond_boule_horizontal(Boule *boule){
	boule->vitesse_y=-1*(boule->vitesse_y);
}

void rebond_boule_vertical(Boule *boule){
	boule->vitesse_x=-1*(boule->vitesse_x);
}

void determine_vitesse_y(Boule *boule){
	boule->vitesse_y=-sqrt(pow(boule->vitesse,2)-pow(boule->vitesse_x,2));
}

bool detecte_collision_plateforme(Plateforme plateforme, Boule boule){
	uint16_t x_gauche=plateforme.x-(plateforme.longueur/2);
	uint16_t x_droite=plateforme.x+(plateforme.longueur/2);
	uint16_t y_haut=plateforme.y-(plateforme.largeur/2);
	uint16_t y_bas=plateforme.y+(plateforme.largeur/2);

	uint16_t x_proche=fmax(x_gauche,fmin(boule.x,x_droite));
	uint16_t y_proche=fmax(y_haut,fmin(boule.y,y_bas));

	uint16_t deltaX=x_proche-boule.x;
	uint16_t deltaY=y_proche-boule.y;
	double distance=sqrt(deltaX*deltaX+deltaY*deltaY);
	return distance<=boule.r;
}

Point point_collision_plateforme(Plateforme plateforme, Boule boule){
	uint16_t x_gauche=plateforme.x-(plateforme.longueur/2);
	uint16_t x_droite=plateforme.x+(plateforme.longueur/2);
	uint16_t y_haut=plateforme.y-(plateforme.largeur/2);
	uint16_t y_bas=plateforme.y+(plateforme.largeur/2);

	Point point_collision;
	point_collision.x=fmax(x_gauche,fmin(boule.x,x_droite));
	point_collision.y=fmax(y_haut,fmin(boule.y,y_bas));

	return point_collision;
}

void collision_plateforme_boule(Plateforme plateforme, Boule *boule){
	bool collision=detecte_collision_plateforme(plateforme,*boule);
	if(collision==true){
		Point point_collision=point_collision_plateforme(plateforme,*boule);
		int16_t x_proportionnel=point_collision.x-plateforme.x;
		double coefficient_angle=0.15;
		boule->vitesse_x=x_proportionnel*coefficient_angle;
		determine_vitesse_y(boule);
	}
}

#define nb_lignes 4
#define nb_colonnes 14
#define nb_briques nb_lignes*nb_colonnes

Brique briques[nb_lignes][nb_colonnes];
uint16_t nb_detruites;

uint16_t choix_couleurs[]={ILI9341_COLOR_BLUE,ILI9341_COLOR_GREEN,ILI9341_COLOR_RED,ILI9341_COLOR_YELLOW};

void init_brique(){
	for(int i=0;i<nb_lignes;i++) {
		for(int j=0;j<nb_colonnes;j++) {
			briques[i][j].etat=true;
			briques[i][j].cote=20;
			briques[i][j].x=6+j*briques[i][j].cote+j*2;
			briques[i][j].y=30+i*briques[i][j].cote+i*2;
			briques[i][j].couleur=choix_couleurs[(i*nb_colonnes+j)%4];
		}
	}
	nb_detruites=0;
}

void afficher_brique(){
	for(int i=0;i<nb_lignes;i++) {
		for(int j=0;j<nb_colonnes;j++) {
			if(briques[i][j].etat==true){
				uint16_t x0=briques[i][j].x;
				uint16_t y0=briques[i][j].y;
				uint16_t x1=briques[i][j].x+briques[i][j].cote;
				uint16_t y1=briques[i][j].y+briques[i][j].cote;
				uint16_t couleur=briques[i][j].couleur;
				ILI9341_DrawFilledRectangle(x0,y0,x1,y1,couleur);
			}
		}
	}
}

bool detecte_collision_brique(Brique brique, Boule boule){
	uint16_t x_gauche=brique.x;
	uint16_t x_droite=brique.x+brique.cote;
	uint16_t y_haut=brique.y;
	uint16_t y_bas=brique.y+brique.cote;

	uint16_t x_proche=fmax(x_gauche,fmin(boule.x,x_droite));
	uint16_t y_proche=fmax(y_haut,fmin(boule.y,y_bas));

	uint16_t deltaX=x_proche-boule.x;
	uint16_t deltaY=y_proche-boule.y;
	double distance=sqrt(deltaX*deltaX+deltaY*deltaY);
	return distance<=boule.r;
}

void effacer_brique(Brique brique){
	uint16_t x0=brique.x;
	uint16_t y0=brique.y;
	uint16_t x1=brique.x+brique.cote;
	uint16_t y1=brique.y+brique.cote;
	ILI9341_DrawFilledRectangle(x0,y0,x1,y1,BGC1);
}

void collision_brique_boule(Boule *boule){
	for(int i=0;i<nb_lignes;i++){
		for(int j=0;j<nb_colonnes;j++){
			if(briques[i][j].etat==true){
				bool collision=detecte_collision_brique(briques[i][j],*boule);
				if(collision==true){
					briques[i][j].etat=false;
					nb_detruites++;
					effacer_brique(briques[i][j]);
					uint16_t x0=briques[i][j].x;
					uint16_t x1=briques[i][j].x+briques[i][j].cote;
					if(boule->x<x0 || boule->x>x1){
						rebond_boule_vertical(boule);
						return;
					}
					else{
						rebond_boule_horizontal(boule);
						return;
					}
				}
			}
		}
	}
}

bool detecte_game_over(Boule boule){
	return boule.y+boule.r+boule.vitesse_y>DIMY;
}

bool detecte_win(){
	return nb_detruites==nb_briques;
}
