#include <furi_hal_resources.h>
#include <furi_hal_bus.h>
#include <furi_hal_gpio.h>
#include <furi.h>

#include <stm32wbxx_ll_pwr.h>

#define TAG "FuriHalResources"

const GpioPin gpio_swdio = {.port = GPIOA, .pin = LL_GPIO_PIN_13};
const GpioPin gpio_swclk = {.port = GPIOA, .pin = LL_GPIO_PIN_14};
const GpioPin gpio_usb_dm = {.port = GPIOA, .pin = LL_GPIO_PIN_11};
const GpioPin gpio_usb_dp = {.port = GPIOA, .pin = LL_GPIO_PIN_12};
const GpioPin gpio_usart_tx = {.port = GPIOA, .pin = LL_GPIO_PIN_9};
const GpioPin gpio_usart_rx = {.port = GPIOA, .pin = LL_GPIO_PIN_10};
const GpioPin gpio_i2c_power_scl = {.port = GPIOB, .pin = LL_GPIO_PIN_8};
const GpioPin gpio_i2c_power_sda = {.port = GPIOB, .pin = LL_GPIO_PIN_9};

const GpioPin gpio_spi_d_sck = {.port = GPIOA, .pin = LL_GPIO_PIN_5};
const GpioPin gpio_spi_d_miso = {.port = GPIOA, .pin = LL_GPIO_PIN_6};
const GpioPin gpio_spi_d_mosi = {.port = GPIOA, .pin = LL_GPIO_PIN_7};
const GpioPin gpio_display_cs = {.port = GPIOA, .pin = LL_GPIO_PIN_4};
const GpioPin gpio_display_di = {.port = GPIOB, .pin = LL_GPIO_PIN_1};
const GpioPin gpio_display_rst_n = {.port = GPIOB, .pin = LL_GPIO_PIN_0};
const GpioPin gpio_sdcard_cs = {.port = GPIOB, .pin = LL_GPIO_PIN_2};

static const GpioPin gpio_ext_pb8 = {.port = GPIOB, .pin = LL_GPIO_PIN_8};
static const GpioPin gpio_ext_pb9 = {.port = GPIOB, .pin = LL_GPIO_PIN_9};
static const GpioPin gpio_ext_pa0 = {.port = GPIOA, .pin = LL_GPIO_PIN_0};
static const GpioPin gpio_ext_pa1 = {.port = GPIOA, .pin = LL_GPIO_PIN_1};
static const GpioPin gpio_ext_pa2 = {.port = GPIOA, .pin = LL_GPIO_PIN_2};
static const GpioPin gpio_ext_pa3 = {.port = GPIOA, .pin = LL_GPIO_PIN_3};
static const GpioPin gpio_ext_pa4 = {.port = GPIOA, .pin = LL_GPIO_PIN_4};
static const GpioPin gpio_ext_pa5 = {.port = GPIOA, .pin = LL_GPIO_PIN_5};
static const GpioPin gpio_ext_pa6 = {.port = GPIOA, .pin = LL_GPIO_PIN_6};
static const GpioPin gpio_ext_pa7 = {.port = GPIOA, .pin = LL_GPIO_PIN_7};
static const GpioPin gpio_ext_pa8 = {.port = GPIOA, .pin = LL_GPIO_PIN_8};
static const GpioPin gpio_ext_pa9 = {.port = GPIOA, .pin = LL_GPIO_PIN_9};

static const GpioPin gpio_ext_pb7 = {.port = GPIOB, .pin = LL_GPIO_PIN_7};
static const GpioPin gpio_ext_pb6 = {.port = GPIOB, .pin = LL_GPIO_PIN_6};
static const GpioPin gpio_ext_pb5 = {.port = GPIOB, .pin = LL_GPIO_PIN_5};
static const GpioPin gpio_ext_pb4 = {.port = GPIOB, .pin = LL_GPIO_PIN_4};
static const GpioPin gpio_ext_pb3 = {.port = GPIOB, .pin = LL_GPIO_PIN_3};
static const GpioPin gpio_ext_pa10 = {.port = GPIOA, .pin = LL_GPIO_PIN_10};
static const GpioPin gpio_ext_pe4 = {.port = GPIOE, .pin = LL_GPIO_PIN_4};
static const GpioPin gpio_ext_pb1 = {.port = GPIOB, .pin = LL_GPIO_PIN_1};
static const GpioPin gpio_ext_pb0 = {.port = GPIOB, .pin = LL_GPIO_PIN_0};
static const GpioPin gpio_ext_pb2 = {.port = GPIOB, .pin = LL_GPIO_PIN_2};
static const GpioPin gpio_ext_pa15 = {.port = GPIOA, .pin = LL_GPIO_PIN_15};

/* No onboard keys are defined on the UBYTE board. */
const InputPin input_pins[1] = {0};
const size_t input_pins_count = 0;

/*
 * Physical GPIOs exposed on the two UBYTE 14-pin headers.
 * Numbering is connector-local: 1..14 for header 1, then 15..25 for the
 * GPIO-bearing positions of header 2; power and ground positions are omitted.
 */
const GpioPinRecord gpio_pins[] = {
    {.pin = &gpio_ext_pb8, .name = "PB8", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 1, .debug = false},
    {.pin = &gpio_ext_pb9, .name = "PB9", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 2, .debug = false},
    {.pin = &gpio_ext_pa0, .name = "PA0", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 3, .debug = false},
    {.pin = &gpio_ext_pa1, .name = "PA1", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 4, .debug = false},
    {.pin = &gpio_ext_pa2, .name = "PA2", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 5, .debug = false},
    {.pin = &gpio_ext_pa3, .name = "PA3", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 6, .debug = false},
    {.pin = &gpio_ext_pa4, .name = "PA4", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 7, .debug = false},
    {.pin = &gpio_ext_pa5, .name = "PA5", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 8, .debug = false},
    {.pin = &gpio_ext_pa6, .name = "PA6", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 9, .debug = false},
    {.pin = &gpio_ext_pa7, .name = "PA7", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 10, .debug = false},
    {.pin = &gpio_ext_pa8, .name = "PA8", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 11, .debug = false},
    {.pin = &gpio_ext_pa9, .name = "PA9", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 12, .debug = false},
    {.pin = &gpio_ext_pb7, .name = "PB7", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 15, .debug = false},
    {.pin = &gpio_ext_pb6, .name = "PB6", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 16, .debug = false},
    {.pin = &gpio_ext_pb5, .name = "PB5", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 17, .debug = false},
    {.pin = &gpio_ext_pb4, .name = "PB4", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 18, .debug = false},
    {.pin = &gpio_ext_pb3, .name = "PB3", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 19, .debug = false},
    {.pin = &gpio_ext_pa10, .name = "PA10", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 20, .debug = false},
    {.pin = &gpio_ext_pe4, .name = "PE4", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 21, .debug = false},
    {.pin = &gpio_ext_pb1, .name = "PB1", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 22, .debug = false},
    {.pin = &gpio_ext_pb0, .name = "PB0", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 23, .debug = false},
    {.pin = &gpio_ext_pb2, .name = "PB2", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 24, .debug = false},
    {.pin = &gpio_ext_pa15, .name = "PA15", .channel = FuriHalAdcChannelNone, .pwm_output = FuriHalPwmOutputIdNone, .number = 25, .debug = false},
};

const size_t gpio_pins_count = COUNT_OF(gpio_pins);

static void furi_hal_resources_init_gpio_pins(GpioMode mode) {
    for(size_t i = 0; i < gpio_pins_count; i++) {
        furi_hal_gpio_init(gpio_pins[i].pin, mode, GpioPullNo, GpioSpeedLow);
    }
}

void furi_hal_resources_init_early(void) {
    furi_hal_bus_enable(FuriHalBusGPIOA);
    furi_hal_bus_enable(FuriHalBusGPIOB);
    furi_hal_bus_enable(FuriHalBusGPIOE);

    /* Force a USB disconnect before peripheral initialization. */
    furi_hal_gpio_write(&gpio_usb_dm, true);
    furi_hal_gpio_write(&gpio_usb_dp, true);
    furi_hal_gpio_init_simple(&gpio_usb_dm, GpioModeOutputOpenDrain);
    furi_hal_gpio_init_simple(&gpio_usb_dp, GpioModeOutputOpenDrain);
    furi_hal_gpio_write(&gpio_usb_dm, false);
    furi_hal_gpio_write(&gpio_usb_dp, false);
    furi_delay_us(5);
    furi_hal_gpio_write(&gpio_usb_dm, true);
    furi_hal_gpio_write(&gpio_usb_dp, true);
    furi_hal_gpio_init_simple(&gpio_usb_dm, GpioModeAnalog);
    furi_hal_gpio_init_simple(&gpio_usb_dp, GpioModeAnalog);

    furi_hal_resources_init_gpio_pins(GpioModeAnalog);

}

void furi_hal_resources_deinit_early(void) {
    furi_hal_resources_init_gpio_pins(GpioModeAnalog);
    furi_hal_bus_disable(FuriHalBusGPIOE);
    furi_hal_bus_disable(FuriHalBusGPIOB);
    furi_hal_bus_disable(FuriHalBusGPIOA);
}

void furi_hal_resources_init(void) {
    FURI_LOG_I(TAG, "UBYTE resources initialized");
}

int32_t furi_hal_resources_get_ext_pin_number(const GpioPin* gpio) {
    for(size_t i = 0; i < gpio_pins_count; i++) {
        if(gpio_pins[i].pin == gpio) return gpio_pins[i].number;
    }
    return -1;
}

const GpioPinRecord* furi_hal_resources_pin_by_name(const char* name) {
    for(size_t i = 0; i < gpio_pins_count; i++) {
        const GpioPinRecord* record = &gpio_pins[i];
        if(strcasecmp(name, record->name) == 0) return record;
    }
    return NULL;
}

const GpioPinRecord* furi_hal_resources_pin_by_number(uint8_t number) {
    for(size_t i = 0; i < gpio_pins_count; i++) {
        const GpioPinRecord* record = &gpio_pins[i];
        if(record->number == number) return record;
    }
    return NULL;
}
