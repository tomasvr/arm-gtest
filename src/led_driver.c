#include <led_driver.h>
#include <runtime_error.h>

static uint16_t *leds_address;
static uint16_t leds_image;

static inline void update_hardware(void)
{
    *leds_address = leds_image;
}

static inline uint16_t led_to_bit(int led_number)
{
    return 1 << (led_number - 1);
}

static inline bool is_out_of_bounds(int led_number)
{
    if ((led_number < 1) || (led_number > 16))
    {
        RUNTIME_ERROR("LED Driver: out-of-bounds LED", led_number);
        return true;
    }
    return false;
}

static inline void set_led_image_bit(int bit)
{
    leds_image |= led_to_bit(bit);
}

static inline void clear_led_image_bit(int bit)
{
    leds_image &= ~led_to_bit(bit);
}

void led_driver_init(uint16_t *address)
{
    leds_address = address;
    leds_image = 0x0;
    update_hardware();
}

uint16_t led_driver_get_state(void)
{
    return leds_image;
}

void led_driver_turn_on_led(int led)
{
    if (is_out_of_bounds(led))
    {
        return;
    }
    set_led_image_bit(led);
    update_hardware();
}
void led_driver_turn_off_led(int led)
{
    if (led < 1 || led > 16)
    {
        return;
    }
    clear_led_image_bit(led);
    update_hardware();
}

void led_driver_turn_on_all(void)
{
    leds_image = ALL_LEDS_ON;
    update_hardware();
}

void led_driver_turn_off_all(void)
{
    leds_image = ALL_LEDS_OFF;
    update_hardware();
}

bool led_driver_led_is_on(int led)
{
    if (is_out_of_bounds(led))
    {
        return false;
    }
    return leds_image & led_to_bit(led);
}