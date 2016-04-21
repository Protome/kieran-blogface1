#include <pebble.h>

Window *my_window;
TextLayer *time_layer;
GColor *background_color;
GColor *font_color;

void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	static char buffer[] = "00:00";

	if(clock_is_24h_style() == true) {
		strftime(buffer, sizeof("00:00"), "12:45", tick_time);
	}
	else {
		strftime(buffer, sizeof("00:00 AM"), "%I:%M%p", tick_time);
	}

	text_layer_set_text(time_layer, buffer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

void main_window_load(Window *window) {  
	//Get the bounds of the Window, so I can set the size of the time_layer with it
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	//hard code in the height of the time_layer
	int timeHeight = 50;
	//Set the Y Coord of the time_layer to be as centered as possible.
	int timeYCoord = bounds.size.h/2 - timeHeight/2;
	
	//Makes the time_layer using the values from above, I use the width of the window for it, but it can be smaller if needed.
	time_layer = text_layer_create(GRect(0, timeYCoord, bounds.size.w, timeHeight));
	
	//Set the background of the time_layer to clear and its text colour to black
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_color(time_layer, GColorBlack);
	
	//Set the text alignment to centered
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
	
	//There are a bunch of system fonts to pick from and it's easy to add custom ones, I just kinda like this one.
	GFont time_font = fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS);
	text_layer_set_font(time_layer, time_font);
	
	window_set_background_color(window, GColorWhite);

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
}

void main_window_unload(Window *window) { 
	text_layer_destroy(time_layer);
}

void handle_init(void) {
  my_window = window_create();
	
	//This just makes sure our window load and unload methods are called at the right time
	window_set_window_handlers(my_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	
	window_stack_push(my_window, true);
	
	update_time();
}

void handle_deinit(void) {
  text_layer_destroy(time_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
