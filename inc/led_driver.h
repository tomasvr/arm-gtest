#include <common.h>

enum
{
    ALL_LEDS_ON = (uint16_t)~0,
    ALL_LEDS_OFF = (uint16_t)~ALL_LEDS_ON
};

void led_driver_init(uint16_t *address);
uint16_t led_driver_get_state();
void led_driver_turn_on_led(int led);
void led_driver_turn_off_led(int led);
void led_driver_turn_on_all(void);
void led_driver_turn_off_all(void);
bool led_driver_led_is_on(int led);
