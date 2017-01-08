#include "keyboard.h"
#include "test.h"

#define KEY_NONE 0xFF
#define KEY_8 0xFD
#define KEY_4 0xFE
#define KEY_6 0xFB
#define KEY_2 0xFD

/**
 * Detecte les touches 8, 4, 6 et 2 comme des fleches.
 * - '8' --> Fleche montante.
 * - '4' --> Fleche vers la gauche.
 * - '6' --> Fleche vers la droite.
 * - '2' --> Fleche vers le bas.
 * @param keyboard Tableau avec l'état des 4 lignes du clavier.
 * @return La fleche correspondante à la touche enfoncée, ou ARROW_NEUTRAL
 * si aucune touche est enfoncée.
 */
Arrow KEYBOARD_readArrows(unsigned char *keyboard) {
	unsigned int n=0;
	Arrow arrow = ARROW_NEUTRAL;
		while((keyboard[n]) || (arrow!=ARROW_NEUTRAL)){
			if(keyboard[n]!=KEY_NONE){
				switch (keyboard[n]){
					case KEY_8:
						arrow = ARROW_UP;
					break;

					case KEY_4:
						arrow = ARROW_LEFT;
					break;

					case KEY_6:
						arrow = ARROW_RIGHT;
					break;
					
					case KEY_2                                                                                              :
						arrow =	ARROW_DOWN;				
					break;
				}
			}
			n++;
		}

	//Si touche multiple 

	return arrow;
}

#ifdef TEST

int testKeyboardArrows() {
	int testsInError = 0;
	unsigned char keys[4] = {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE};

	testsInError += assertEquals(KEYBOARD_readArrows(keys), ARROW_NEUTRAL, "KB001");
	
	keys[0] = KEY_8;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_UP, "KB002");
	keys[0] = KEY_NONE;
	keys[1] = KEY_4;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_LEFT, "KB003");
	keys[1] = KEY_6;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_RIGHT, "KB004");
	keys[1] = KEY_NONE;
	keys[2] = KEY_2;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_DOWN, "KB005");

	return testsInError;
}

int testKeyboard() {
	int testsInError = 0;

	testsInError += testKeyboardArrows();
	
	return testsInError;
}

#endif
