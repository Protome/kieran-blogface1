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
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	int timeHeight = 50;
	int timeYCoord = bounds.size.h/2 - timeHeight/2;
	
	time_layer = text_layer_create(GRect(0, timeYCoord, bounds.size.w, timeHeight));
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_color(time_layer, GColorBlack);
	
	window_set_background_color(window, GColorWhite);

	GFont time_font = fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS);
	text_layer_set_font(time_layer, time_font);
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
}

void main_window_unload(Window *window) { 
	text_layer_destroy(time_layer);
}

void handle_init(void) {
  my_window = window_create();
	
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
