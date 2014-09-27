#include <pebble.h>

/* TODO
 * Some persistence of some sort
 * Interface with phone (Android app)
 */

static Window *window;
static TextLayer *current_hole_text_layer;
static TextLayer *current_stroke_text_layer;
static char hole_label[6] = "Hole ";
static int current_hole = 1;
static char current_hole_string[2];
static char stroke_label[8] = "Stroke ";
static int strokes = 0;
static char current_strokes_string[2];

static void update_current_hole_label() {
  static char label[10];
  strncpy(label, hole_label, sizeof(hole_label));
  char current_hole_label[3];
  snprintf(current_hole_label, sizeof(current_hole_label), "%d", current_hole);
  strncat(label, current_hole_label, sizeof(current_hole_label));
  text_layer_set_text(current_hole_text_layer, label);
}

static void update_current_stroke_label() {
  static char label[10];
  strncpy(label, stroke_label, sizeof(stroke_label));
  char current_stroke_label[3];
  snprintf(current_stroke_label, sizeof(current_stroke_label), "%d", strokes);
  strncat(label, current_stroke_label, sizeof(current_stroke_label));
  text_layer_set_text(current_stroke_text_layer, label);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole < 18) {
    current_hole += 1;
  }
  update_current_hole_label();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (strokes <= 9) {
    strokes += 1;
  }
  update_current_stroke_label();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (strokes >= 1) {
    strokes -= 1;
  }
  update_current_stroke_label();
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole > 1) {
    current_hole -= 1;
  }
  update_current_hole_label();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  current_hole_text_layer = text_layer_create((GRect) {
    .origin = { 0, 30 },
    .size = { bounds.size.w, 30 }
  });
  text_layer_set_font(current_hole_text_layer, fonts_get_system_font(
        FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(current_hole_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(current_hole_text_layer));

  current_stroke_text_layer = text_layer_create((GRect) {
    .origin = { 0, 80 },
    .size = { bounds.size.w, 30 }
  });
  text_layer_set_font(current_stroke_text_layer, fonts_get_system_font(
        FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(current_stroke_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(current_stroke_text_layer));

  text_layer_set_text(current_stroke_text_layer, "Stroke 1");
  text_layer_set_text(current_hole_text_layer, "Hole 1");
}

static void window_unload(Window *window) {
  text_layer_destroy(current_hole_text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
