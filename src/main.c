#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_week_layer;
static TextLayer *s_bt_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_day_layer;
static TextLayer *s_daynumb_layer;
int changeCounter;

#define TK   GColorFromRGB(0,255,0)
#define BL   GColorBlack
#define CL   GColorClear
#define TLST  text_layer_set_text

  static void switchBlack(Window *window) {
    window_set_background_color(window, BL);
}

static void switchClear(Window *window) {
    window_set_background_color(window, CL);
}


static void update_battery_info() {
  static char s_battery_buffer[8];
BatteryChargeState charge_state = battery_state_service_peek();
if (charge_state.is_charging) {
  //snprintf(s_battery_buffer, sizeof(s_battery_buffer), "charging");
  TLST(s_battery_layer, "^^%");
} else {
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", charge_state.charge_percent);
  TLST(s_battery_layer, s_battery_buffer);
}
  
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  
//int hour = tick_time->tm_hour;
//  if ((hour > 8) | (hour < 18)) { switchClear(s_main_window); }
//  else { switchBlack(s_main_window); }
  
}

static void update_date() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), "%d/%m" , tick_time);
  
  static char s_w_buffer[8];
  strftime(s_w_buffer, sizeof(s_w_buffer), "%%W: %W", tick_time);
  
  static char s_day_buffer[10];
  strftime(s_day_buffer, sizeof(s_day_buffer), "%A", tick_time);
  
  static char s_daynumb_buffer[8];
  strftime(s_daynumb_buffer, sizeof(s_daynumb_buffer), "%%j: %j", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_date_layer, s_buffer);
  text_layer_set_text(s_week_layer, s_w_buffer);
  text_layer_set_text(s_day_layer, s_day_buffer);
  text_layer_set_text(s_daynumb_layer, s_daynumb_buffer);
  update_battery_info();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  changeCounter++;
  //
  //if (changeCounter == 5) {
  if (tick_time->tm_hour == 0) {
    if (tick_time->tm_min == 0) {
      update_date();
    }
   }
  //changeCounter=0; }
}



static void main_window_load(Window *window) {
  // Get information about the Window
  window_set_background_color(window, GColorBlack);
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_date_layer = text_layer_create(
      GRect(0,PBL_IF_ROUND_ELSE(5,5),bounds.size.w,50));
 
   s_week_layer = text_layer_create(
      GRect(0,PBL_IF_ROUND_ELSE(25,25),bounds.size.w,50));
  
     s_day_layer = text_layer_create(
      GRect(0,PBL_IF_ROUND_ELSE(42,42),bounds.size.w,50));
  
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(60, 60), bounds.size.w, 50));

    s_daynumb_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(100, 100), bounds.size.w, 50));
  
      s_battery_layer = text_layer_create(
      GRect(0,PBL_IF_ROUND_ELSE(120,120),bounds.size.w,50));
  
   s_bt_layer = text_layer_create(
      GRect(0,PBL_IF_ROUND_ELSE(140,140),bounds.size.w,50));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, BL);
  text_layer_set_text_color(s_time_layer,TK);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  text_layer_set_background_color(s_date_layer, GColorClear );
  text_layer_set_text_color(s_date_layer, TK);
  text_layer_set_text(s_date_layer, "00/00");
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
    text_layer_set_background_color(s_week_layer, GColorClear );
  text_layer_set_text_color(s_week_layer, TK); 
  text_layer_set_text(s_week_layer, "%W: X");
  text_layer_set_font(s_week_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_week_layer, GTextAlignmentCenter);
  
      text_layer_set_background_color(s_daynumb_layer, GColorClear );
  text_layer_set_text_color(s_daynumb_layer,TK); 
  text_layer_set_text(s_daynumb_layer, "%j: X");
  text_layer_set_font(s_daynumb_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_daynumb_layer, GTextAlignmentCenter);
  
      text_layer_set_background_color(s_day_layer, GColorClear );
  text_layer_set_text_color(s_day_layer, TK);
  text_layer_set_text(s_day_layer, "Someday");
  text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

      text_layer_set_background_color(s_bt_layer, GColorClear );
  text_layer_set_text_color(s_bt_layer, TK);
  #ifdef PBL_SDK_2
if (bluetooth_connection_service_peek()) {
#elif PBL_SDK_3
if (connection_service_peek_pebble_app_connection()) {
#endif
    text_layer_set_text(s_bt_layer, "BT: V");
} else {
    text_layer_set_text(s_bt_layer, "BT: X"); }
  text_layer_set_font(s_bt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_bt_layer, GTextAlignmentCenter);

    text_layer_set_background_color(s_battery_layer, GColorClear );
  text_layer_set_text_color(s_battery_layer,TK);
  text_layer_set_text(s_battery_layer, "XX%");
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);

  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
      layer_add_child(window_layer, text_layer_get_layer(s_week_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_bt_layer));
    layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
      layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
        layer_add_child(window_layer, text_layer_get_layer(s_daynumb_layer));


}
  


static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_week_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_bt_layer);
    text_layer_destroy(s_battery_layer);
    text_layer_destroy(s_day_layer);    
  text_layer_destroy(s_daynumb_layer);
}

void bt_handler(bool connected) {
  if (connected) {
      text_layer_set_text(s_bt_layer,"BT: V");
  } else {
      text_layer_set_text(s_bt_layer, "BT: V");
  }
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  changeCounter=0;
  // Make sure the time is displayed from the start
  update_time();
  update_date();
  update_battery_info();
  
  #ifdef PBL_SDK_2
bluetooth_connection_service_subscribe(bt_handler);
#elif PBL_SDK_3
connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bt_handler
});
#endif

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
 // tick_timer_service_subscribe(DAY_UNIT, day_tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}