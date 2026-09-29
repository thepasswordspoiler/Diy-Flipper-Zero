#include "flipper.pb.h"
#include "rpc_i.h"
#include "gpio.pb.h"
#include <furi_hal_gpio.h>
#include <furi_hal_power.h>
#include <power/power_service/power.h>

/*
 * UBYTE RPC GPIO pins.
 * These remain private to rpc_gpio.c and are not exported through
 * furi_hal_resources.h.
 */
static const GpioPin rpc_gpio_pb2 = {
    .port = GPIOB,
    .pin = LL_GPIO_PIN_2
};

static const GpioPin rpc_gpio_pb3 = {
    .port = GPIOB,
    .pin = LL_GPIO_PIN_3
};

static const GpioPin rpc_gpio_pa4 = {
    .port = GPIOA,
    .pin = LL_GPIO_PIN_4
};

static const GpioPin rpc_gpio_pa6 = {
    .port = GPIOA,
    .pin = LL_GPIO_PIN_6
};

static const GpioPin rpc_gpio_pa7 = {
    .port = GPIOA,
    .pin = LL_GPIO_PIN_7
};

static const GpioPin* rpc_pin_to_hal_pin(PB_Gpio_GpioPin rpc_pin) {
    switch(rpc_pin) {
    case PB_Gpio_GpioPin_PB2:
        return &rpc_gpio_pb2;
    case PB_Gpio_GpioPin_PB3:
        return &rpc_gpio_pb3;
    case PB_Gpio_GpioPin_PA4:
        return &rpc_gpio_pa4;
    case PB_Gpio_GpioPin_PA6:
        return &rpc_gpio_pa6;
    case PB_Gpio_GpioPin_PA7:
        return &rpc_gpio_pa7;

    /*
     * These protobuf GPIOs exist in the generic Flipper RPC interface,
     * but GPIOC0/C1/C3 are not bonded to the UBYTE expansion headers.
     * Keep the enum exhaustive while preserving the existing unreachable
     * behavior for unsupported pins.
     */
    case PB_Gpio_GpioPin_PC0:
    case PB_Gpio_GpioPin_PC1:
    case PB_Gpio_GpioPin_PC3:
        __builtin_unreachable();
    }

    __builtin_unreachable();
}
static GpioMode rpc_mode_to_hal_mode(PB_Gpio_GpioPinMode rpc_mode) {
    switch(rpc_mode) {
    case PB_Gpio_GpioPinMode_OUTPUT:
        return GpioModeOutputPushPull;
    case PB_Gpio_GpioPinMode_INPUT:
        return GpioModeInput;
    }

    __builtin_unreachable();
}

static GpioPull rpc_pull_mode_to_hall_pull_mode(PB_Gpio_GpioInputPull pull_mode) {
    switch(pull_mode) {
    case PB_Gpio_GpioInputPull_UP:
        return GpioPullUp;
    case PB_Gpio_GpioInputPull_DOWN:
        return GpioPullDown;
    case PB_Gpio_GpioInputPull_NO:
        return GpioPullNo;
    }

    __builtin_unreachable();
}

static void rpc_system_gpio_set_pin_mode(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_set_pin_mode_tag);

    RpcSession* session = context;

    PB_Gpio_SetPinMode cmd = request->content.gpio_set_pin_mode;
    const GpioPin* pin = rpc_pin_to_hal_pin(cmd.pin);
    GpioMode mode = rpc_mode_to_hal_mode(cmd.mode);

    furi_hal_gpio_init_simple(pin, mode);
    if(mode == GpioModeOutputPushPull) {
        furi_hal_gpio_write(pin, false);
    }

    rpc_send_and_release_empty(session, request->command_id, PB_CommandStatus_OK);
}

static void rpc_system_gpio_write_pin(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_write_pin_tag);

    RpcSession* session = context;

    PB_Gpio_WritePin cmd = request->content.gpio_write_pin;
    const GpioPin* pin = rpc_pin_to_hal_pin(cmd.pin);
    uint8_t value = !!(cmd.value);

    PB_Main* response = malloc(sizeof(PB_Main));
    response->command_id = request->command_id;
    response->has_next = false;

    if(LL_GPIO_MODE_OUTPUT != LL_GPIO_GetPinMode(pin->port, pin->pin)) {
        response->command_status = PB_CommandStatus_ERROR_GPIO_MODE_INCORRECT;
    } else {
        response->command_status = PB_CommandStatus_OK;
        furi_hal_gpio_write(pin, value);
    }

    rpc_send_and_release(session, response);

    free(response);
}

static void rpc_system_gpio_read_pin(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_read_pin_tag);

    RpcSession* session = context;

    PB_Gpio_ReadPin cmd = request->content.gpio_read_pin;
    const GpioPin* pin = rpc_pin_to_hal_pin(cmd.pin);

    PB_Main* response = malloc(sizeof(PB_Main));
    response->command_id = request->command_id;
    response->has_next = false;

    if(LL_GPIO_MODE_INPUT != LL_GPIO_GetPinMode(pin->port, pin->pin)) {
        response->command_status = PB_CommandStatus_ERROR_GPIO_MODE_INCORRECT;
    } else {
        response->command_status = PB_CommandStatus_OK;
        response->which_content = PB_Main_gpio_read_pin_response_tag;
        response->content.gpio_read_pin_response.value = !!furi_hal_gpio_read(pin);
    }

    rpc_send_and_release(session, response);

    free(response);
}

void rpc_system_gpio_get_pin_mode(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_get_pin_mode_tag);

    RpcSession* session = context;

    PB_Gpio_GetPinMode cmd = request->content.gpio_get_pin_mode;
    const GpioPin* pin = rpc_pin_to_hal_pin(cmd.pin);

    PB_Main* response = malloc(sizeof(PB_Main));
    response->command_id = request->command_id;
    response->has_next = false;

    uint32_t raw_pin_mode = LL_GPIO_GetPinMode(pin->port, pin->pin);

    PB_Gpio_GpioPinMode pin_mode;
    if(LL_GPIO_MODE_INPUT == raw_pin_mode) {
        pin_mode = PB_Gpio_GpioPinMode_INPUT;
        response->command_status = PB_CommandStatus_OK;
    } else if(LL_GPIO_MODE_OUTPUT == raw_pin_mode) {
        pin_mode = PB_Gpio_GpioPinMode_OUTPUT;
        response->command_status = PB_CommandStatus_OK;
    } else {
        pin_mode = PB_Gpio_GpioPinMode_INPUT;
        response->command_status = PB_CommandStatus_ERROR_GPIO_UNKNOWN_PIN_MODE;
    }

    response->which_content = PB_Main_gpio_get_pin_mode_response_tag;
    response->content.gpio_get_pin_mode_response.mode = pin_mode;

    rpc_send_and_release(session, response);

    free(response);
}

void rpc_system_gpio_set_input_pull(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_set_input_pull_tag);

    RpcSession* session = context;

    PB_Gpio_SetInputPull cmd = request->content.gpio_set_input_pull;
    const GpioPin* pin = rpc_pin_to_hal_pin(cmd.pin);
    const GpioPull pull_mode = rpc_pull_mode_to_hall_pull_mode(cmd.pull_mode);

    PB_Main* response = malloc(sizeof(PB_Main));
    response->command_id = request->command_id;
    response->has_next = false;

    PB_CommandStatus status;
    if(LL_GPIO_MODE_INPUT != LL_GPIO_GetPinMode(pin->port, pin->pin)) {
        status = PB_CommandStatus_ERROR_GPIO_MODE_INCORRECT;
    } else {
        status = PB_CommandStatus_OK;
        furi_hal_gpio_init(pin, GpioModeInput, pull_mode, GpioSpeedLow);
    }

    rpc_send_and_release_empty(session, request->command_id, status);

    free(response);
}

void rpc_system_gpio_get_otg_mode(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_get_otg_mode_tag);

    RpcSession* session = context;

    const bool otg_enabled = furi_hal_power_is_otg_enabled();

    PB_Main* response = malloc(sizeof(PB_Main));
    response->command_id = request->command_id;
    response->which_content = PB_Main_gpio_get_otg_mode_response_tag;
    response->content.gpio_get_otg_mode_response.mode = otg_enabled ? PB_Gpio_GpioOtgMode_ON :
                                                                      PB_Gpio_GpioOtgMode_OFF;

    rpc_send_and_release(session, response);

    free(response);
}

void rpc_system_gpio_set_otg_mode(const PB_Main* request, void* context) {
    furi_assert(request);
    furi_assert(context);
    furi_assert(request->which_content == PB_Main_gpio_set_otg_mode_tag);

    RpcSession* session = context;

    const PB_Gpio_GpioOtgMode mode = request->content.gpio_set_otg_mode.mode;

    Power* power = furi_record_open(RECORD_POWER);

    if(mode == PB_Gpio_GpioOtgMode_OFF) {
        power_enable_otg(power, false);
    } else {
        power_enable_otg(power, true);
    }

    furi_record_close(RECORD_POWER);

    rpc_send_and_release_empty(session, request->command_id, PB_CommandStatus_OK);
}

void* rpc_system_gpio_alloc(RpcSession* session) {
    furi_assert(session);

    RpcHandler rpc_handler = {
        .message_handler = NULL,
        .decode_submessage = NULL,
        .context = session,
    };

    rpc_handler.message_handler = rpc_system_gpio_set_pin_mode;
    rpc_add_handler(session, PB_Main_gpio_set_pin_mode_tag, &rpc_handler);

    rpc_handler.message_handler = rpc_system_gpio_write_pin;
    rpc_add_handler(session, PB_Main_gpio_write_pin_tag, &rpc_handler);

    rpc_handler.message_handler = rpc_system_gpio_read_pin;
    rpc_add_handler(session, PB_Main_gpio_read_pin_tag, &rpc_handler);

    rpc_handler.message_handler = rpc_system_gpio_get_pin_mode;
    rpc_add_handler(session, PB_Main_gpio_get_pin_mode_tag, &rpc_handler);

    rpc_handler.message_handler = rpc_system_gpio_set_input_pull;
    rpc_add_handler(session, PB_Main_gpio_set_input_pull_tag, &rpc_handler);

    rpc_handler.message_handler = rpc_system_gpio_get_otg_mode;
    rpc_add_handler(session, PB_Main_gpio_get_otg_mode_tag, &rpc_handler);

    rpc_handler.message_handler = rpc_system_gpio_set_otg_mode;
    rpc_add_handler(session, PB_Main_gpio_set_otg_mode_tag, &rpc_handler);

    return NULL;
}
