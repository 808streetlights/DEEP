/*
 * jeu.h
 *
 *  Created on: 29 nov. 2023
 *      Author: maind
 */

#ifndef JEU_H_
#define JEU_H_

void ecran_titre();
void initialisation_menu_principal();
void initialisation_difficulte();
void initialisation_jeu(Plateforme *plateforme, Boule *boule);
void deroulement_jeu(Plateforme *plateforme, Boule *boule);
void initialisation_game_over();
void initialisation_win();
void deplacements_menus();

#endif /* JEU_H_ */
