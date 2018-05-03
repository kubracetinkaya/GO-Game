#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<allegro5\allegro.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_primitives.h>
#include<string.h>

#define SCREENWIDTH 1000
#define SCREENHEIGHT 1000
#define COLORWHITE al_map_rgb(255,255,255)
const int scaleRatio = 5;               //taþýn boyutunu deðiþtir derse burayla oyna

enum GAMESTATE {GAMESTATE_PLAY, GAMESTATE_EXIT};
enum NODESTATE {BLACK, WHITE, EMPTY};

typedef struct PIXELCOORDINATES {
	int x;
	int y;
}COORD;

int initAllegro() {
	if (!al_init()) {
		printf("Allegro initialization failed!");
		return 0;
	}
	if (!al_install_mouse()) {
		printf("Mouse installation failed!");
		al_rest(5);
		return 0;
	}
	if (!al_init_image_addon()) {
		printf("Image addon initialization failed!");
		al_rest(5);
		return 0;
	}
	if (!al_init_font_addon()) {
		printf("Font addon initialization failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_ttf_addon()) {
		printf("TTF addon initialization failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_primitives_addon()) {
		printf("Primitives addon initialization failed!");
		al_rest(5);
		return 0;
	}
}

void drawBoard() {
	for (int i = 1; i <= 9; i++) {
		al_draw_line(SCREENWIDTH / 10, SCREENHEIGHT * i / 10, SCREENWIDTH * 9 / 10, SCREENHEIGHT * i / 10, COLORWHITE, 1);
		al_draw_line(SCREENWIDTH * i / 10, SCREENHEIGHT / 10, SCREENWIDTH * i / 10, SCREENHEIGHT * 9 / 10, COLORWHITE, 1);
	}
}
COORD closestNode (ALLEGRO_MOUSE_STATE mouse){
	COORD out;
	int x = mouse.x + SCREENWIDTH/20;
	int y = mouse.y + SCREENHEIGHT/20;

	x /= SCREENWIDTH/10;
	y /= SCREENHEIGHT/13;
	
	out.x = ceil(x);
	out.y = ceil(y);

	return out;
}
void clickHandle(NODESTATE (*arr)[9][9],ALLEGRO_MOUSE_STATE state, int* turn) {
	COORD closest;
	if (al_mouse_button_down(&state, 1)) {
		closest = closestNode(state);
		if ((*arr)[closest.x - 1][closest.y - 1] == EMPTY) {
			if (*turn == 0) {
				(*arr)[closest.x - 1][closest.y - 1] = BLACK;
			}
			else if (*turn == 1) {
				(*arr)[closest.x - 1][closest.y - 1] = WHITE;
			}
			*turn = !(*turn);                    //taþlarýn hepsi ayný renk olsun derse ! kaldýr
		}
	}
}
COORD nodeToPixel(COORD in) {
	COORD out;
	out.x = in.x * SCREENWIDTH / 10;
	out.y = in.y * SCREENHEIGHT / 10;

	return out;
}
void drawTransStone(ALLEGRO_MOUSE_STATE mouse, ALLEGRO_BITMAP *stone) {
	COORD closest = closestNode(mouse);
	COORD nodePixels = nodeToPixel(closest);

	int stoneWidth = al_get_bitmap_width(stone);
	int stoneHeight = al_get_bitmap_height(stone);
	int scaledWidth = stoneWidth / scaleRatio;
	int scaledHeight = stoneHeight / scaleRatio;
	
	if(closest.x >= 1 && closest.x <= 9 && closest.y >= 1 && closest.y <= 9)
		al_draw_scaled_bitmap(stone, 0, 0, stoneWidth, stoneHeight, nodePixels.x - scaledWidth / 2, nodePixels.y - scaledHeight / 2, scaledWidth, scaledHeight, 0);
}
void drawStones(NODESTATE arr[9][9], ALLEGRO_BITMAP* blackStone, ALLEGRO_BITMAP* whiteStone) {
	int stoneWidth = al_get_bitmap_width(blackStone);
	int stoneHeight = al_get_bitmap_height(blackStone);
	int scaledWidth = stoneWidth / scaleRatio;
	int scaledHeight = stoneHeight / scaleRatio;

	COORD node;
	COORD nodePixels;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			node.x = i + 1;
			node.y = j + 1;
			nodePixels = nodeToPixel(node);
			if (arr[i][j] == BLACK)
				al_draw_scaled_bitmap(blackStone, 0, 0, stoneWidth, stoneHeight, nodePixels.x - scaledWidth / 2, nodePixels.y - scaledHeight / 2, scaledWidth, scaledHeight, 0);
			else if (arr[i][j] == WHITE) {
				al_draw_scaled_bitmap(whiteStone, 0, 0, stoneWidth, stoneHeight, nodePixels.x - scaledWidth / 2, nodePixels.y - scaledHeight / 2, scaledWidth, scaledHeight, 0);
			}
		}
	}
}
void stateCheck(NODESTATE t_array[9][9] , int* const levelCounter) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			NODESTATE checkfor;
			if (t_array[i][j] == EMPTY)
				continue;
			else if (t_array[i][j] == BLACK)
				checkfor = WHITE;
			else
				checkfor = BLACK;


			if(t_array[i-1][j] == checkfor &&
				t_array[i+1][j] == checkfor &&
				t_array[i][j-1] == checkfor &&
				t_array[i][j+1] == checkfor) {
				t_array[i][j] = EMPTY;
				//score buradan artýyor.
				(*levelCounter)++;
				

			}

		}
	}
}
int main() {
	//Allegro initialization
	if (!initAllegro()) { return EXIT_FAILURE; }
	
	ALLEGRO_DISPLAY *display = al_create_display(SCREENWIDTH, SCREENHEIGHT);
	al_set_window_position(display, 0, 0);
	ALLEGRO_FONT *arcade = al_load_ttf_font("ARCADECLASSIC.TTF", 40, 0);
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_MOUSE_STATE mouseState;

	ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
	ALLEGRO_EVENT myEvent;
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	
	ALLEGRO_BITMAP *blackStone = al_load_bitmap("black_stone.png");
	ALLEGRO_BITMAP *whiteStone = al_load_bitmap("white_stone.png");

	//Game logic initialization

	int levelCounter = 0; //score
	int turn = 0; 	//0 for black 1 for white
	GAMESTATE gameState = GAMESTATE_PLAY;
	NODESTATE nodes[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			nodes[i][j] = EMPTY;
		}
	}
	
	al_start_timer(timer);
	while (gameState == GAMESTATE_PLAY) {
		al_wait_for_event(eventQueue, &myEvent);
		if (myEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			gameState = GAMESTATE_EXIT;
		al_get_mouse_state(&mouseState);
		clickHandle(&nodes, mouseState, &turn);
		stateCheck(nodes, &levelCounter);
		//drawing
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(arcade, al_map_rgb(255, 0, 0), 500, 10, ALLEGRO_ALIGN_CENTRE, "LEVEL   %d", levelCounter);
		drawBoard();
		drawTransStone(mouseState, turn == 0 ? blackStone : whiteStone);
		drawStones(nodes, blackStone, whiteStone);
		al_flip_display();
	}
	return 0;
}