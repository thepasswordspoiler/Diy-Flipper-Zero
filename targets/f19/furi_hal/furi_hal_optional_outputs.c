#include <stdbool.h>
#include <stdint.h>

#include <furi_hal_speaker.h>
#include <furi_hal_vibro.h>

/*
 * UBYTE STM32WB55 Stage-1 hardware:
 *
 * No dedicated Flipper-style speaker or vibration motor is connected yet.
 *
 * These private compatibility implementations keep the common notification
 * service linkable without exporting unsupported F7 hardware APIs.
 */

/* Speaker compatibility -------------------------------------------------- */

bool furi_hal_speaker_acquire(uint32_t timeout) {
    (void)timeout;
    return false;
}

void furi_hal_speaker_release(void) {
}

bool furi_hal_speaker_is_mine(void) {
    return false;
}

void furi_hal_speaker_start(float frequency, float volume) {
    (void)frequency;
    (void)volume;
}

void furi_hal_speaker_stop(void) {
}

/* Vibro compatibility --------------------------------------------------- */

void furi_hal_vibro_on(bool value) {
    (void)value;
}
