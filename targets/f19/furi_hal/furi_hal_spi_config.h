#pragma once

#include <furi_hal_spi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** CC1101/OLED/SD-capable shared SPI1 preset at 8 MHz, mode 0. */
extern const LL_SPI_InitTypeDef furi_hal_spi_preset_1edge_low_8m;

/** SD-card slow-init preset (~2 MHz). */
extern const LL_SPI_InitTypeDef furi_hal_spi_preset_1edge_low_2m;

/** SD-card fast I/O preset (~16 MHz). */
extern const LL_SPI_InitTypeDef furi_hal_spi_preset_1edge_low_16m;

/** Shared UBYTE SPI1 bus on PA5/PA6/PA7 with software CS. */
extern FuriHalSpiBus furi_hal_spi_bus_d;

/** Generic external SPI handle for the shared UBYTE SPI1 bus. */
extern const FuriHalSpiBusHandle furi_hal_spi_bus_handle_external;

/** Shared SPI1 display handle for the externally wired OLED. */
extern const FuriHalSpiBusHandle furi_hal_spi_bus_handle_display;

/** SD card slow-init handle on shared UBYTE SPI1, CS=PB2. */
extern const FuriHalSpiBusHandle furi_hal_spi_bus_handle_sd_slow;

/** SD card fast-I/O handle on shared UBYTE SPI1, CS=PB2. */
extern const FuriHalSpiBusHandle furi_hal_spi_bus_handle_sd_fast;

#ifdef __cplusplus
}
#endif
