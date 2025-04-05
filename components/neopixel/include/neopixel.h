#include "led_strip.h"
#include "led_strip_rmt.h"
#include "led_strip_types.h"

// Zdefiniowac liczbe paskow, liczbe ledow na pasku i wartosc skoku po kolorach
#define STRIP_NUM 6
#define LED_NUM 24
#define HUE_BUMP 25

extern led_strip_config_t conf;
extern led_strip_rmt_config_t rmt_conf;

void configure_strip(led_strip_handle_t *strip, uint8_t leds, uint8_t pin);

void backlight(led_strip_handle_t *strip, uint8_t brightness);

void animate(led_strip_handle_t *strip);