#pragma once

#include <furi.h>
#include <furi_hal_adc.h>
#include <furi_hal_pwm.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Input API remains available, but this board has no onboard Flipper-style keys. */
#define INPUT_DEBOUNCE_TICKS 4

typedef enum {
    InputKeyUp,
    InputKeyDown,
    InputKeyRight,
    InputKeyLeft,
    InputKeyOk,
    InputKeyBack,
    InputKeyMAX,
} InputKey;

typedef enum {
    LightRed = (1 << 0),
    LightGreen = (1 << 1),
    LightBlue = (1 << 2),
    LightBacklight = (1 << 3),
} Light;

typedef struct {
    const GpioPin* gpio;
    const InputKey key;
    const bool inverted;
    const char* name;
} InputPin;

typedef struct {
    const GpioPin* pin;
    const char* name;
    const FuriHalAdcChannel channel;
    const FuriHalPwmOutputId pwm_output;
    const uint8_t number;
    const bool debug;
} GpioPinRecord;

/* Board-level fixed connections. */
extern const GpioPin gpio_swdio;
extern const GpioPin gpio_swclk;
extern const GpioPin gpio_usb_dm;
extern const GpioPin gpio_usb_dp;
extern const GpioPin gpio_usart_tx;
extern const GpioPin gpio_usart_rx;
extern const GpioPin gpio_i2c_power_scl;
extern const GpioPin gpio_i2c_power_sda;

extern const GpioPin gpio_spi_d_sck;
extern const GpioPin gpio_spi_d_miso;
extern const GpioPin gpio_spi_d_mosi;

/* External OLED wiring contract: CS=PA4, D/C=PB1, RST=PB0. */
extern const GpioPin gpio_display_cs;
extern const GpioPin gpio_display_di;
extern const GpioPin gpio_display_rst_n;
extern const GpioPin gpio_sdcard_cs;

/* UBYTE expansion-header GPIO resources. */

extern const InputPin input_pins[];
extern const size_t input_pins_count;

extern const GpioPinRecord gpio_pins[];
extern const size_t gpio_pins_count;

void furi_hal_resources_init_early(void);
void furi_hal_resources_deinit_early(void);
void furi_hal_resources_init(void);

int32_t furi_hal_resources_get_ext_pin_number(const GpioPin* gpio);
const GpioPinRecord* furi_hal_resources_pin_by_name(const char* name);
const GpioPinRecord* furi_hal_resources_pin_by_number(uint8_t number);

#ifdef __cplusplus
}
#endif
