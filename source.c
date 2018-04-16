#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"

int main(int argc, char **argv) 
{

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP  *image = NULL;
	al_init_primitives_addon();
	if (!al_init()) 
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	if (!al_init_image_addon()) 
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	display = al_create_display(1000, 1000);


	if (!display)
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	image = al_load_bitmap("image.jpg");

	if (!image)
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}
	
	al_draw_bitmap(image,0,0, 0);
	float x_1, y_1, x_2, y_2;
	x_1 = 100; y_1 = 100; x_2 = 900; y_2 = y_1;

	for (int i = 0; i < 9; i++)
	{
		al_draw_line(x_1, y_1, x_2, y_2, al_map_rgb(0, 0, 0), 1.0);
		y_1 = y_1 + 100; y_2 = y_1;
        }

	y_1 = 100;
	y_2 = 900;
	x_1 = 100;
	x_2 = x_1;
	for (int i = 0; i < 9; i++) 
	{
		al_draw_line(x_1, y_1, x_2, y_2, al_map_rgb(0, 0, 0), 1.0);
		x_1 = x_1 + 100; x_2 = x_1;
	}
	
	//al_draw_rectangle(10.0, 10.0, 500.0, 400, al_map_rgb(0, 0, 0), 1.0);

	while (1) 
	{
		al_flip_display();
		al_rest(2);

		//al_destroy_display(display);
		//al_destroy_bitmap(image);

	}
	

	return 0;
}
