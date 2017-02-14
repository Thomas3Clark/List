/*
  NicCageQuotes
  An amalgamation of the example image watchapp and the guide to show text in a watchapp.
  Displays a picture of Nicolas Cage.
  Prints a movie quote said by one of his characters each time a button is pressed.
  Praised be the one true god, Nicolas Cage.
*/

#include <pebble.h>
#include <stdlib.h>
#include <time.h>

static Window *s_main_window;
static Window *s_text_window;
static Window *s_image_window;

static BitmapLayer *image_layer;

static TextLayer *s_output_layer;

static GBitmap *image;

// stuff here is for what is effectively a string array of cage quotes
char quote0[] = "Oh yeah, you're right.";
char quote1[] = "Down the road.";
char quote2[] = "Holy macaroni...";
char quote3[] = "Oh gee, I'm sorry";
char quote4[] = "I don't even know.";
char quote5[] = "Stop bending time and space for your own sick desires.";
char quote6[] = "You will die alone, having never been loved.";
char quote7[] = "Throw me in the river and end my misery.";
char quote8[] = "Stop your ways, evildoers.";
char quote9[] = "Eskimo Lives Matter";
char *quotes[10] = {quote0, quote1, quote2, quote3, quote4, quote5, quote6, quote7, quote8, quote9};

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  int rng = rand() % 10;
  vibes_short_pulse();
  Layer *window_layer = window_get_root_layer(s_text_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  s_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, quotes[rng]);
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
  window_stack_remove(s_text_window, true);
  window_stack_push(s_text_window, true);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  int rng = rand() % 10;
  vibes_short_pulse();
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  s_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, quotes[rng]);
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void main_window_load(Window *window) {
  srand((unsigned) time(NULL));
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // This needs to be deinited on app exit which is when the event loop ends
  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NIC_CAGE);

  // The bitmap layer holds the image for display
  image_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));

}

static void text_window_load(Window *window) {
  
  
}

static void main_window_unload(Window *window) {

  gbitmap_destroy(image);

  bitmap_layer_destroy(image_layer);
  text_layer_destroy(s_output_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  //s_image_window = window_create();
  //window_set_window_handlers(s_image_window, (WindowHandlers) {
  //  .load = image_window_load,
  //  .unload = image_window_unload
  //});
  
  s_text_window = window_create();
  window_set_window_handlers(s_text_window, (WindowHandlers) {
    .load = text_window_load,
    .unload = text_window_unload
  });
  
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_click_config_provider(s_image_window, click_config_provider);
  window_set_click_config_provider(s_text_window, click_config_provider);
  
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
  window_destroy(s_image_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
