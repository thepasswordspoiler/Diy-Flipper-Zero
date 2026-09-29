# DIY Flipper Zero — UBYTE STM32WB55 / F19

Custom Flipper Zero firmware port for the UBYTE STM32WB55 evaluation platform.

## Current status

- Target: F19
- Hardware: UBYTE STM32WB55
- Firmware: 0.1.1
- qFlipper USB CDC: working
- RPC / screen stream: working
- USB DFU flashing: working
- Factory keys: not implemented
- SD card: not implemented

## Upstream

This project is based on the official Flipper Zero firmware project:
https://github.com/flipperdevices/flipperzero-firmware

Known development base:
`7f0b6e1c14431708cfde75ae1ba13df59e868041`

The repository keeps UBYTE-specific changes separate from upstream so that future firmware updates can be reconciled rather than blindly flashing official Flipper images onto the custom hardware.

## Important

This is a custom hardware port. Do not flash an official Flipper Zero firmware image directly to the UBYTE board.

## Development

See the documentation under `docs/` for build, flash, hardware, and update-manager information.
