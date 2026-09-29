#include <furi_hal_i2c_config.h>
#include <furi_hal_resources.h>
#include <furi_hal_gpio.h>
#include <furi_hal_bus.h>
#include <furi.h>
#include <stm32wbxx_ll_i2c.h>

#define TAG "FuriHalI2cConfig"

static FuriMutex* furi_hal_i2c_power_mutex = NULL;
static FuriMutex* furi_hal_i2c_external_mutex = NULL;

static void furi_hal_i2c_bus_power_event_callback(FuriHalI2cBus* bus, FuriHalI2cBusEvent event) {
    if(event == FuriHalI2cBusEventInit) {
        furi_hal_i2c_power_mutex = furi_mutex_alloc(FuriMutexTypeNormal);
        bus->current_handle = NULL;
    } else if(event == FuriHalI2cBusEventDeinit) {
        furi_mutex_free(furi_hal_i2c_power_mutex);
        furi_hal_i2c_power_mutex = NULL;
    } else if(event == FuriHalI2cBusEventLock) {
        furi_check(furi_mutex_acquire(furi_hal_i2c_power_mutex, FuriWaitForever) == FuriStatusOk);
    } else if(event == FuriHalI2cBusEventUnlock) {
        furi_check(furi_mutex_release(furi_hal_i2c_power_mutex) == FuriStatusOk);
    } else if(event == FuriHalI2cBusEventActivate) {
        furi_hal_bus_enable(FuriHalBusI2C1);
    } else if(event == FuriHalI2cBusEventDeactivate) {
        furi_hal_bus_disable(FuriHalBusI2C1);
    }
}

static void furi_hal_i2c_bus_external_event_callback(FuriHalI2cBus* bus, FuriHalI2cBusEvent event) {
    if(event == FuriHalI2cBusEventInit) {
        furi_hal_i2c_external_mutex = furi_mutex_alloc(FuriMutexTypeNormal);
        bus->current_handle = NULL;
    } else if(event == FuriHalI2cBusEventDeinit) {
        furi_mutex_free(furi_hal_i2c_external_mutex);
        furi_hal_i2c_external_mutex = NULL;
    } else if(event == FuriHalI2cBusEventLock) {
        furi_check(furi_mutex_acquire(furi_hal_i2c_external_mutex, FuriWaitForever) == FuriStatusOk);
    } else if(event == FuriHalI2cBusEventUnlock) {
        furi_check(furi_mutex_release(furi_hal_i2c_external_mutex) == FuriStatusOk);
    } else if(event == FuriHalI2cBusEventActivate) {
        furi_hal_bus_enable(FuriHalBusI2C1);
    } else if(event == FuriHalI2cBusEventDeactivate) {
        furi_hal_bus_disable(FuriHalBusI2C1);
    }
}

static void furi_hal_i2c_handle_callback(const FuriHalI2cBusHandle* handle, FuriHalI2cBusHandleEvent event) {
    if(event == FuriHalI2cBusHandleEventDeactivate) {
        LL_I2C_Disable(I2C1);
        furi_hal_gpio_init(&gpio_i2c_power_scl, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        furi_hal_gpio_init(&gpio_i2c_power_sda, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        UNUSED(handle);
        return;
    }

    if(event != FuriHalI2cBusHandleEventActivate) return;

    UNUSED(handle);
    furi_hal_gpio_init_ex(&gpio_i2c_power_scl, GpioModeAltFunctionOpenDrain, GpioPullNo, GpioSpeedLow, GpioAltFn4I2C1);
    furi_hal_gpio_init_ex(&gpio_i2c_power_sda, GpioModeAltFunctionOpenDrain, GpioPullNo, GpioSpeedLow, GpioAltFn4I2C1);

    LL_I2C_InitTypeDef init = {0};
    init.PeripheralMode = LL_I2C_MODE_I2C;
    init.Timing = 0x10707DBC;
    init.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
    init.DigitalFilter = 0;
    init.OwnAddress1 = 0;
    init.TypeAcknowledge = LL_I2C_ACK;
    init.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
    furi_check(LL_I2C_Init(I2C1, &init) == SUCCESS);
    LL_I2C_Enable(I2C1);
}

FuriHalI2cBus furi_hal_i2c_bus_power = {
    .i2c = I2C1,
    .current_handle = NULL,
    .callback = furi_hal_i2c_bus_power_event_callback,
};

FuriHalI2cBus furi_hal_i2c_bus_external = {
    .i2c = I2C1,
    .current_handle = NULL,
    .callback = furi_hal_i2c_bus_external_event_callback,
};

const FuriHalI2cBusHandle furi_hal_i2c_handle_power = {
    .bus = &furi_hal_i2c_bus_power,
    .callback = furi_hal_i2c_handle_callback,
};

const FuriHalI2cBusHandle furi_hal_i2c_handle_external = {
    .bus = &furi_hal_i2c_bus_external,
    .callback = furi_hal_i2c_handle_callback,
};
