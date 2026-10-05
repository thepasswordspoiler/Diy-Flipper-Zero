# UBYTE F19 / DIY Flipper Zero — Project Checkpoint
Date: 2026-10-05

## Goal
Build a custom Flipper-Zero-like device around the UBYTE STM32WB55CGU6 evaluation board using official Flipper firmware as the base, with an honest custom Target 19/F19 identity. Main milestones: working qFlipper USB/RPC, OLED/UI, external microSD-backed `/ext` storage, F19-compatible FAP apps, then PN532/RDM6300/CC1101/audio peripherals. Factory/enclave keys are unavailable and must not be faked.

## Current hardware
- UBYTE STM32WB55CGU6 eval board: dual-core M4/M0+, 1MB Flash, 256KB RAM, 3.3V, USB-C, external ST-Link, two 14-pin headers.
- SmartElex 1.54" SSD1309 OLED, 128x64, SPI/I2C.
- RDM6300 125kHz EM4100 RFID reader, UART 9600, 5V.
- CC1101 SMA transceiver, SPI, 1.8–3.6V.
- MP3-TF-16P / DFPlayer-style serial audio module with its own TF controller.
- 9x15cm double-sided prototype PCB.
- 5-way tactile switch: Up/Down/Left/Right/Center.
- ST-Link V2.
- PN532 NFC/RFID module, I2C/SPI/HSU.
- TTP223 capacitive touch module, being used as the 6th UI switch.
- Micro SD SPI adapter module with pins 3V3, CS, MOSI, CLK, MISO, GND.

## Established UBYTE pin map
- USB D- = PA11
- USB D+ = PA12
- SWDIO = PA13
- SWCLK = PA14
- USART TX = PA9
- USART RX = PA10
- I2C SCL = PB8
- I2C SDA = PB9
- SPI SCK = PA5
- SPI MISO = PA6
- SPI MOSI = PA7
- OLED CS = PA4
- OLED D/C = PB1
- OLED RST = PB0
- SD CS = PB2 (new physical wiring)

Important: PA4 is already OLED CS, so SD must not use PA4 for CS. SD and OLED can share PA5/PA6/PA7 with separate chip-selects.

## Current SD wiring
Latest user-confirmed:
- SD 3V3 -> UBYTE 3.3V
- SD GND -> UBYTE GND
- SD CS -> PB2
- SD MISO -> PA6
- SD MOSI -> PA7
- SD CLK/SCK was not restated in the latest message; previous/required SPI SCK is PA5. Verify physically that SD CLK -> PA5 before testing.

Intended final SD wiring:
3V3 -> 3.3V
GND -> GND
CS -> PB2
CLK -> PA5
MISO -> PA6
MOSI -> PA7

## Firmware base / known-good checkpoint
Local repo:
`C:\p\flipperzero-firmware`

Base upstream checkpoint:
- Commit: `7f0b6e1c14431708cfde75ae1ba13df59e868041`
- Tag/version: `0.1.1`

Last verified custom device:
- Name: UBYTE-F1
- Hardware model: UBYTE STM32WB55
- Target: 19
- Protobuf: 0.25
- API: 91.0
- Firmware version: 0.1.1
- qFlipper: 1.3.3
- USB VCP: 0483:5740
- COM port: COM3
- RPC + screen streaming: working

Do not call this custom firmware 1.4.3; the custom project checkpoint is 0.1.1.

## F19 changes already completed
- Created `targets/f19/` for custom Target 19.
- `targets/f19/api_symbols.csv` uses API 91.0; SDK check reports API 91.0 up to date.
- F19 target inherits F7 infrastructure with custom linker/SDK/resource configuration.
- Unsupported modules/HAL pieces were excluded for this hardware, including NFC, LF RFID, Sub-GHz, iButton, infrared, and the GPIO application module pieces.
- F19 resources map USB/SWD/USART/I2C/SPI to the UBYTE pins above.
- No onboard Flipper-style 5-key input array; F19 input pin count was set to 0.
- Optional speaker/vibro compatibility was made no-op.
- Serial uses private PA2/PA3 definitions.
- Resource early init configures expansion pins and performs USB reconnect.
- F19 SD HAL is currently only a stub.

### Current F19 SD HAL blocker
`targets/f19/furi_hal/furi_hal_sd.c` currently:
- reports SD not present (`furi_hal_sd_is_present() == false`)
- returns errors from SD init
- returns errors for block read/write/info/state
- mount retry count is 0

This is the current reason the storage service cannot make `/ext` ready.

## Storage / FAP status
F19 firmware builds successfully. Earlier target tests reached 23/23 passing.

Snake app:
`.\fbt TARGET_HW=19 fap_snake_game`
succeeds:
- SDKCHK: API 91.0 up to date
- APPCHK succeeds
- `snake_game.fap` was produced at:
  `C:\p\flipperzero-firmware\build\f19-firmware-D\.extapps\snake_game.fap`
- Previously observed size: 6900 bytes

USB FAP deployment using COM3 reached the device but failed because storage was not ready:
`python scripts/runfap.py -p COM3 -s build\f19-firmware-D\.extapps\snake_game.fap -t /int/apps/snake_game.fap`

Result:
`Error: Storage error: path '/int/apps': filesystem not ready`

The correct next storage target is `/ext` after SD is implemented and mounted. `storage_ext.c` uses FATFS and the `furi_hal_sd_*` API.

An earlier broad FAP build failed on `example_thermo` because it references `gpio_ibutton`, which is not defined for UBYTE. Treat this as a target-specific example limitation, not an SD failure.

## App compatibility finding
The FAP loader checks manifest, hardware target ID, firmware API compatibility and imports. Keep custom Target 19/API 91 explicit. Do not spoof an official Flipper target/hardware/version just to bypass Flipper Lab compatibility UI.

## Programming / debug
- USB DFU: WORKING.
- STM32CubeProgrammer previously recognized STM32WB5x/35xx, device ID 0x0495, 1MB.
- ST-Link SWD previously failed with core-ID/no-target errors despite ~3.29V; USB DFU and qFlipper are the known-good programming/communication paths.

## Git / GitHub
Local repo:
`C:\p\flipperzero-firmware`

Current remotes shown by user:
`diy` -> `https://github.com/thepasswordspoiler/Diy-Flipper-Zero.git`
`origin` -> `https://github.com/flipperdevices/flipperzero-firmware.git`

The user tried:
`git fetch origin feature/ubyte-f19-sd-spi`
and
`git switch feature/ubyte-f19-sd-spi`

Both failed because that branch/reference does not exist on `origin`. The user also tried to add `diy` again and Git said it already exists. Do not fetch `feature/ubyte-f19-sd-spi` from origin unless a real branch is created.

Known custom branch:
`port/ubyte-stm32wb55`

Known custom commit:
`29d5acd5d430a53604799d20aecde835ec40df25`
Message: `port: add UBYTE STM32WB55 F19 target`
Parent: `7f0b6e1c14431708cfde75ae1ba13df59e868041`

A merge to DIY main was previously made (known prefix `76d9b920`; inspect local Git for exact current history).

## Patches previously used
- `/Downloads/ubyte-f19-stage1.patch`
- `/Downloads/ubyte-f19-sdk-api-fix.patch`
- `/Downloads/ubyte-f19-build-fix.patch`
- `/Downloads/ubyte-f19-pa15-resource-fix.patch`
- DIY repo patch: `firmware/patches/ubyte-f19/0001-current-ubyte-f19.patch`

## Next engineering task: real SD support
1. Verify SD CLK is physically on PA5.
2. Inspect current F19 SD HAL/resource files and the upstream SD HAL implementation.
3. Keep shared SPI bus PA5/PA6/PA7 and use PB2 as SD CS; preserve OLED CS on PA4.
4. Replace the F19 `furi_hal_sd.c` stub with a real STM32WB SPI microSD implementation compatible with the existing FATFS/storage service.
5. Implement SD initialization/presence behavior for this 6-pin module; no dedicated card-detect pin was reported.
6. Build F19.
7. Flash by USB DFU.
8. Boot and verify SD-backed storage becomes ready and `/ext` mounts.
9. Install/test `snake_game.fap` at `/ext/apps/snake_game.fap`.
10. Then integrate the 5-way switch + TTP223 touch and later PN532/RDM6300/CC1101/audio.

## Critical constraints
- Preserve OLED functionality.
- Never use PA4 as SD CS.
- Never fake SD presence.
- Never fake factory/enclave keys.
- Keep Target 19 and custom hardware identity explicit.
- Make small, testable firmware changes and Git checkpoints.

## Resume commands for a new chat
Run from PowerShell:
`cd C:\p\flipperzero-firmware`
`git status --short --branch`
`git remote -v`
`git branch --all`
`git log --oneline --decorate -10`

Then inspect:
- `targets/f19/target.json`
- `targets/f19/furi_hal/furi_hal_sd.c`
- F19 resource files
- upstream/reference SD HAL
- `applications/services/storage/storage_ext.c`
- `applications/services/storage/storage.c`

Then verify SD CLK -> PA5 physically before powering/testing.
