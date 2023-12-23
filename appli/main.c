/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "MatrixKeyboard/matrix_keyboard.h"
#include "gestion_affichage.h"
#include "jeu_base.h"
#include "jeu.h"

void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool_e readButton(void)
{
	return !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
}

static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;
}


int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms � la liste des fonctions appel�es automatiquement chaque ms par la routine d'interruption du p�riph�rique SYSTICK
	Systick_add_callback_function(&process_ms);

	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_1);
	ILI9341_Fill(ILI9341_COLOR_BLACK);

	/*
	ILI9341_DrawCircle(20,20,5,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);
	ILI9341_Putc(110,11,'x',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Putc(15,110,'y',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	*/

	//Diff�rentes tailles : &Font_7x10, &Font_11x18, &Font_16x26
	//ILI9341_Puts(centrer_x("Bonjour"),centrer_y("Bonjour"),"Bonjour", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_RED);

	Plateforme plateforme;
	Boule boule;

	//initialisation_jeu(&plateforme, &boule);

	matrix_keyboard_demo_process_main();

	void state_machine(void){

		typedef enum{
			INIT=0,
			MENU,
			JEU,
			PAUSE,
			DIFFICULTE,
			GAME_OVER,
			WIN,
		}state_e;

		static state_e state=INIT;

	    switch(state){
	    	case INIT:
	    		ecran_titre();
	    		if(KEYBOARD_get_key()=='5'){
	    			state=MENU;
	    			effacer_ecran(ILI9341_COLOR_BLACK);
	    			break;
	    		}
	    		break;
	    	case MENU:
	    		initialisation_menu_principal();
	    		while(1){
	    			deplacements_menus();
	    			if(KEYBOARD_get_key()=='5'){
	    				if(getIndiceBoutonActif()==0){
	    					state=JEU;
	    					effacer_ecran(ILI9341_COLOR_BLACK);
	    					break;
	    				}
	    				if(getIndiceBoutonActif()==1){
	    					state=DIFFICULTE;
	    					effacer_ecran(ILI9341_COLOR_BLACK);
	    					break;
	    				}
	    			}
	    		}
	    		break;
	    	case JEU:
	    		initialisation_jeu(&plateforme, &boule);
	    		while(1){
	    			deroulement_jeu(&plateforme, &boule);
	    			if(detecte_game_over(boule)){
	    				state=GAME_OVER;
	    				effacer_ecran(ILI9341_COLOR_BLACK);
	    				break;
	    			}
	    			if(detecte_win()){
	    				state=WIN;
	    				effacer_ecran(ILI9341_COLOR_BLACK);
	    				break;
	    			}
	    		}
	    		break;
	    	//case PAUSE:
	    	case DIFFICULTE:
	    		initialisation_difficulte();
	    		while(1){
	    			deplacements_menus();
	    			if(KEYBOARD_get_key()=='5'){
						if(getIndiceBoutonActif()==0){
							uint16_t new_vitesse_difficulte=3;
							adapter_difficulte(new_vitesse_difficulte);
							state=JEU;
							effacer_ecran(ILI9341_COLOR_BLACK);
							break;
						}
						if(getIndiceBoutonActif()==1){
							uint16_t new_vitesse_difficulte=4;
							adapter_difficulte(new_vitesse_difficulte);
							state=JEU;
							effacer_ecran(ILI9341_COLOR_BLACK);
							break;
						}
						if(getIndiceBoutonActif()==2){
							uint16_t new_vitesse_difficulte=5;
							adapter_difficulte(new_vitesse_difficulte);
							state=JEU;
							effacer_ecran(ILI9341_COLOR_BLACK);
							break;
						}
	    			}
	    		}
	    		break;
	    	case GAME_OVER:
	    		initialisation_game_over();
	    		while(1){
	    			deplacements_menus();
	    			if(KEYBOARD_get_key()=='5'){
	    				if(getIndiceBoutonActif()==0){
	    				    state=JEU;
	    				    effacer_ecran(ILI9341_COLOR_BLACK);
	    				    break;
	    				}
	    				if(getIndiceBoutonActif()==1){
	    					state=MENU;
	    					effacer_ecran(ILI9341_COLOR_BLACK);
	    					break;
	    				}
	    			}
	    		}
	    		break;
	    	case WIN:
	    		initialisation_win();
	    		while(1){
	    			deplacements_menus();
	    			if(KEYBOARD_get_key()=='5'){
	    				if(getIndiceBoutonActif()==0){
	    				    state=JEU;
	    				    effacer_ecran(ILI9341_COLOR_BLACK);
	    				    break;
	    				}
	    				if(getIndiceBoutonActif()==1){
	    					state=MENU;
	    					effacer_ecran(ILI9341_COLOR_BLACK);
	    					break;
	    				}
	    			}
	    		}
	    		break;
	    	default:
	    		break;
	    }
	}



	while(1)	//boucle de t�che de fond
	{
		state_machine();
		/*
		if(KEYBOARD_get_key()=='2'){
			monter_bouton();
		}
		if(KEYBOARD_get_key()=='8'){
			descendre_bouton();
		}
		*/
		//deroulement_jeu(&plateforme, &boule);
	}
}
