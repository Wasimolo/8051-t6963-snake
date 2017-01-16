#include "test.h"
#include "bdd.h"
#include "t6963c.h"
#include "buffer.h"
#include "gameboard.h"
#include "snake.h"

#define FRUIT_CALORIES 3

/**
 * Modifie les coordonn�es du serpent selon sa direction.
 * @param snake La description du serpent.
 */
void SNAKE_move(Snake *snake) {
	switch (snake->direction) {
		case MOVES_UP:
				snake->position.y --;
		break;
	
		case MOVES_DOWN:
				snake->position.y ++;
		break;
	
		case MOVES_LEFT:
				snake->position.x --;
		break;
		
		case MOVES_RIGHT:
			snake->position.x ++;
		break;
	}
}

/**
 * D�cide si le serpent vit ou meurt, ou mange un fruit, selon
 * sa position et ce qui se trouve � cet endroit.
 * @param snake La description du serpent.
 */
void SNAKE_liveOrDie(Snake *snake) {
	unsigned char c = 0;
	c=T6963C_readFrom(snake->position.x, snake->position.y);
	
	switch (c){
		case FRUIT:
			snake->status=EATING;
			snake->caloriesLeft = FRUIT_CALORIES;
			break;
		case EMPTY:
			if(((snake->position.x > SNAKE_LIMIT_X0) && ((snake->position.x < SNAKE_LIMIT_X1))) && ((snake->position.y > SNAKE_LIMIT_Y0 ) && (snake->position.y < SNAKE_LIMIT_Y1 ))){
				snake->status=ALIVE;
			}else{
				snake->status=DEAD;
			}
			break;
		case SNAKE_BODY:
			snake->status=DEAD;
			break;
		case SNAKE_SWALLOW:
			snake->status=DEAD;
			break;
		case OBSTACLE_A:
			snake->status=DEAD;
			break;
		case OBSTACLE_B:
			snake->status=DEAD;
		break;
		case OBSTACLE_C:
			snake->status=DEAD;
		break;
		case OBSTACLE_D:
			snake->status=DEAD;
			break;
		case OBSTACLE_E:
			snake->status=DEAD;
		break;
		case OBSTACLE_F:
			snake->status=DEAD;
		break;
		case OBSTACLE_G:
			snake->status=DEAD;
		break;
		case OBSTACLE_H:
			snake->status=DEAD;
		break;
		default: 
			snake->status=ALIVE;
			break;
		}

/*#define OBSTACLE_A (0x21 + CHAR_BASE)
#define OBSTACLE_B (0x22 + CHAR_BASE)
#define OBSTACLE_C (0x23 + CHAR_BASE)
#define OBSTACLE_D (0x24 + CHAR_BASE)
#define OBSTACLE_E (0x25 + CHAR_BASE)
#define OBSTACLE_F (0x26 + CHAR_BASE)
#define OBSTACLE_G (0x27 + CHAR_BASE)
#define OBSTACLE_H*/
}

/**
 * Affiche la t�te du serpent.
 * @param snake La d�finition du serpent.
 */
void SNAKE_showHead(Snake *snake) {
			
	if (snake->status != DEAD){
			T6963C_writeAt(snake->position.x,snake->position.y,SNAKE_HEAD);
		}else{
			T6963C_writeAt(snake->position.x,snake->position.y,SNAKE_DEAD);
		}
			
}

/**
 * Affiche le corps du serpent, et efface la queue du serpent.
 * Aussi, si le serpent a mang� un fruit, affiche un caract�re sp�cial.
 * @param snake La d�finition du serpent.
 */
void SNAKE_showBody(Snake *snake) {
	unsigned char nbr_corps=0;
	unsigned char x=0, y=0;
	unsigned int i=0;

		BUFFER_in(snake->position.x);
		BUFFER_in(snake->position.y);
		if (snake->status == EATING){
			T6963C_writeAt(snake->position.x,snake->position.y,SNAKE_SWALLOW);
		}else{
			T6963C_writeAt(snake->position.x,snake->position.y,SNAKE_BODY);
		}
			//Initialise le snake avec un corps 
			if(snake->caloriesLeft> 0){ //snake->status = ALIVE  &&
			
				snake->caloriesLeft--;
			}else{	

					x = BUFFER_out();
					y = BUFFER_out();
					T6963C_writeAt(x,y,EMPTY);

			}
		
			

	
	
}

/**
 * D�cide de varier la direction du serpent selon la direction indiqu�e.
 * Le serpent ne peut jamais tourner de 180� en un mouvement.
 * @param snake La description du serpent.
 * @param arrow La direction d�sir�e.
 Interdits
	Gauche ->
 */
void SNAKE_turn(Snake *snake, Arrow arrow) {

switch (arrow) {
		case ARROW_UP:
				if(snake->direction == MOVES_DOWN){
					snake->direction=MOVES_DOWN;
				}else{
					snake->direction=MOVES_UP;
				}
		break;
	
		case ARROW_DOWN:
				if(snake->direction == MOVES_UP){
					snake->direction=MOVES_UP;
				}else{
					snake->direction=MOVES_DOWN;
				}
		break;
	
		case ARROW_LEFT:
				if(snake->direction == MOVES_RIGHT){
					snake->direction=MOVES_RIGHT;
				}else{
					snake->direction=MOVES_LEFT;
				}
		break;
		
		case ARROW_RIGHT:
				if(snake->direction == MOVES_LEFT){
					snake->direction=MOVES_LEFT;
				}else{
					snake->direction=MOVES_RIGHT;
				}
		break;
		
		case ARROW_NEUTRAL:
				snake->direction=snake->direction;
		break;
	}
}

/**
 * Effectue une it�ration dans la vie du serpent.
 * @param snake La d�finition du serpent.
 * @return L'�tat du serpent apr�s l'it�ration.
 */
Status SNAKE_iterate(Snake *snake, Arrow arrow) {
	SNAKE_showBody(snake);
	SNAKE_turn(snake, arrow);
	SNAKE_move(snake);
	SNAKE_liveOrDie(snake);
	SNAKE_showHead(snake);

	return snake->status;
}

#ifdef TEST

// ========================== Tests unitaires =================================
// Chaque test v�rifie le comportement d'une fonctionnalit� en �tablissant
// un �tat initial et en v�rifiant l'�tat final.
int testSnakeTurnsTo(Direction currentDirection, Arrow turn, Direction expectedResult, char *testCode) {
	Snake snake = {MOVES_RIGHT, {10, 10}, ALIVE, 0};
	snake.direction = currentDirection;
	SNAKE_turn(&snake, turn);
	return assertEquals(expectedResult, snake.direction, testCode);	
}

int testSnakeTurns() {
	int testsInError = 0;

	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_RIGHT,	MOVES_RIGHT,"ST01");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_LEFT,	MOVES_RIGHT,"ST02");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_UP,		MOVES_UP, 	"ST03");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_DOWN, 	MOVES_DOWN, "ST04");

	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_RIGHT, 	MOVES_LEFT, "ST11");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_LEFT, 	MOVES_LEFT, "ST12");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_UP, 		MOVES_UP, 	"ST13");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_DOWN, 	MOVES_DOWN, "ST14");

	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_RIGHT, 	MOVES_RIGHT,"ST21");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_LEFT, 		MOVES_LEFT, "ST22");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_UP, 		MOVES_UP, 	"ST23");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_DOWN, 		MOVES_UP, 	"ST24");

	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_RIGHT, 	MOVES_RIGHT,"ST31");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_LEFT, 	MOVES_LEFT, "ST32");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_UP, 		MOVES_DOWN, "ST33");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_DOWN, 	MOVES_DOWN, "ST34");

	return testsInError;
}

int testSnakeEatsAFruit() {
	int testsInError = 0;
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 0};
	
	T6963C_writeAt(BDD_SCREEN_X, BDD_SCREEN_Y, FRUIT);
	SNAKE_liveOrDie(&snake);

	testsInError += assertEquals(snake.status, EATING, "SF001");
	testsInError += assertEquals(snake.caloriesLeft, FRUIT_CALORIES, "SF002");

	T6963C_writeAt(BDD_SCREEN_X, BDD_SCREEN_Y, EMPTY);
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, ALIVE, "SF003");

	return testsInError;	
}

int testSnakeMoves() {
	int testsInError = 0;
	Snake snake;

	snake.position.x = 10;
	snake.position.y = 10;

	snake.direction = MOVES_UP;
	SNAKE_move(&snake);
	testsInError += assertEquals(9, snake.position.y, "SM001");

	snake.direction = MOVES_DOWN;
	SNAKE_move(&snake);
	testsInError += assertEquals(10, snake.position.y, "SM002");

	snake.direction = MOVES_LEFT;
	SNAKE_move(&snake);
	testsInError += assertEquals( 9, snake.position.x, "SM003");

	snake.direction = MOVES_RIGHT;
	SNAKE_move(&snake);
	testsInError += assertEquals(10, snake.position.x, "SM004");

	return testsInError;
}

int testSnakeHitsABorder() {
	int testsInError = 0;

	Snake snake;

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0 + 1;
	snake.position.y = SNAKE_LIMIT_Y0 + 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, ALIVE, "SO01");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0;
	snake.position.y = SNAKE_LIMIT_Y0 + 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO02");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0 + 1;
	snake.position.y = SNAKE_LIMIT_Y0;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO03");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X1;
	snake.position.y = SNAKE_LIMIT_Y1 - 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO04");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X1 - 1;
	snake.position.y = SNAKE_LIMIT_Y1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO05");

	return testsInError;
}

// =========================== Tests de comportement ============================
// Chaque test:
// 1- �tablit un �tat initial.
// 2- Simule un sc�nario qu'un utilisateur pourrait r�aliser manuellement.
// 3- V�rifie, en contr�lant le contenu de l'�cran, que ce 
//    que percevrait l'utilisateur est juste

int bddSnakeHitsItsTail() {
	BddExpectedContent c = {
		" 14111....",
		"..1..1....",
		"..1111....",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 11};
	char n;

	BUFFER_clear();
	BDD_clear();
	
	for (n = 0; n < 5; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	for (n = 0; n < 2; n++) {
		SNAKE_iterate(&snake, ARROW_DOWN);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_LEFT);
	}
	for (n = 0; n < 2; n++) {
		SNAKE_iterate(&snake, ARROW_UP);
	}

	return BDD_assert(c, "SNT");
}


int bddSnakeHitsThisObstacle(char obstacle, char *testId) {
	BddExpectedContent c = {
		"  1114....",
		"..........",
		"..........",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	T6963C_writeAt(BDD_SCREEN_X + 5, BDD_SCREEN_Y, obstacle);
	
	for (n = 0; n < 5; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}

	return BDD_assert(c, testId);
}

int bddSnakeHitsAnObstacle() {
	int testsInError = 0;

	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_A, "SNO-A");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_B, "SNO-B");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_C, "SNO-C");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_D, "SNO-D");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_E, "SNO-E");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_F, "SNO-F");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_G, "SNO-G");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_H, "SNO-H");

	return testsInError;
}

int bddSnakeMovesTurnsAndCatchesAFruit() {
	BddExpectedContent c = {
		"      1...",
		"......2...",
		"......1...",
		"...3111...",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	T6963C_writeAt(BDD_SCREEN_X + 6, BDD_SCREEN_Y + 1, FRUIT);
	
	for (n = 0; n < 6; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_DOWN);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_LEFT);
	}

	return BDD_assert(c, "SNTF");
}

int bddSnakeGrows() {
	BddExpectedContent c = {
		"    1113..",
		"..........",
		"..........",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	for (n=0; n<7; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
		//assertEquals(snake.status, ALIVE, "88");
	}
	
	return BDD_assert(c, "SNG");
}

/**
 * Collection de tests.
 * Les tests en erreur sont affich�s � l'�cran.
 * @return Le nombre de tests �chou�s.
 */
int testSnake() {
	int testsInError = 0;

	// Tests unitaires:
	testsInError += testSnakeTurns();
	testsInError += testSnakeMoves();
	testsInError += testSnakeHitsABorder();
	testsInError += testSnakeEatsAFruit();

	// Tests de comportement:
	testsInError += bddSnakeGrows();
	testsInError += bddSnakeMovesTurnsAndCatchesAFruit();
	testsInError += bddSnakeHitsAnObstacle();
	testsInError += bddSnakeHitsItsTail();

	// Nombre de tests en erreur:
	return testsInError;
}

#endif
