#include <furi_hal_light.h>
#include <furi_hal_resources.h>
#include <furi.h>
#include <stm32wbxx_ll_gpio.h>

static const GpioPin gpio_rgb_red = {.port = GPIOB, .pin = LL_GPIO_PIN_4};
static const GpioPin gpio_rgb_green = {.port = GPIOB, .pin = LL_GPIO_PIN_5};
static const GpioPin gpio_rgb_blue = {.port = GPIOA, .pin = LL_GPIO_PIN_15};

static bool state_red = false;
static bool state_green = false;
static bool state_blue = false;

static void ubyte_light_apply(Light light, uint8_t value) {
    const bool on = value != 0;
    if(light & LightRed) { state_red = on; furi_hal_gpio_write(&gpio_rgb_red, on); }
    if(light & LightGreen) { state_green = on; furi_hal_gpio_write(&gpio_rgb_green, on); }
    if(light & LightBlue) { state_blue = on; furi_hal_gpio_write(&gpio_rgb_blue, on); }
}

void furi_hal_light_init(void) {
    furi_hal_gpio_init(&gpio_rgb_red, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_init(&gpio_rgb_green, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_init(&gpio_rgb_blue, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    ubyte_light_apply(LightRed | LightGreen | LightBlue, 0);
}

void furi_hal_light_set(Light light, uint8_t value) { ubyte_light_apply(light, value); }
void furi_hal_light_blink_start(Light light, uint8_t brightness, uint16_t on_time, uint16_t period) {
    UNUSED(on_time); UNUSED(period); ubyte_light_apply(light, brightness);
}
void furi_hal_light_blink_stop(void) { ubyte_light_apply(LightRed | LightGreen | LightBlue, 0); }
void furi_hal_light_blink_set_color(Light light) { ubyte_light_apply(LightRed | LightGreen | LightBlue, 0); ubyte_light_apply(light, 0xFF); }

void furi_hal_light_sequence(const char* sequence) {
    if(!sequence) return;
    while(*sequence) {
        switch(*sequence++) {
        case 'R': ubyte_light_apply(LightRed, 0xFF); break;
        case 'r': ubyte_light_apply(LightRed, 0); break;
        case 'G': ubyte_light_apply(LightGreen, 0xFF); break;
        case 'g': ubyte_light_apply(LightGreen, 0); break;
        case 'B': ubyte_light_apply(LightBlue, 0xFF); break;
        case 'b': ubyte_light_apply(LightBlue, 0); break;
        case 'W': break;
        case 'w': break;
        case '.': furi_delay_ms(250); break;
        case '-': furi_delay_ms(500); break;
        default: break;
        }
    }
}
