# DIY Flipper Zero — UBYTE STM32WB55 / F19

Custom Flipper Zero firmware port and hardware project for the UBYTE STM32WB55 platform.

## Current repository state

- Main: complete upstream firmware history plus the UBYTE F19 integration merge.
- UBYTE development branch: `port/ubyte-stm32wb55`
- UBYTE port commit: `29d5acd5d430a53604799d20aecde835ec40df25`
- Upstream parent: `7f0b6e1c14431708cfde75ae1ba13df59e868041`
- Target: F19 / 19
- Hardware: UBYTE STM32WB55
- Firmware checkpoint: 0.1.1
- API checkpoint: 91.0
- Protobuf: 0.25

## Verified development capabilities

- UBYTE F19 firmware boots on the STM32WB55 board.
- USB CDC enumerates with the VCP identity used by qFlipper.
- qFlipper RPC communication and screen streaming work.
- USB DFU flashing works.
- The UBYTE target test suite has 23 tests and the known-good checkpoint passed 23/23.

## Hardware scope

The UBYTE design uses board-specific resources rather than pretending the hardware is an official Flipper Zero. Hardware features must be enabled only when the physical circuit exists and is electrically verified.

The current firmware stage intentionally does not implement factory keys or an SD-card interface that is not physically present.

## Applications

The UBYTE firmware is a custom target. Official Flipper Lab application availability is therefore not automatically equivalent to official Flipper Zero firmware compatibility. The project will use explicit Target-19 application compatibility rather than spoofing official hardware metadata.

## Updates and safety

Never flash an official Flipper Zero image directly to the UBYTE board.

Future upstream updates should follow:

```
official upstream
      ↓
UBYTE/F19 patch reconciliation
      ↓
conflict detection
      ↓
F19 build
      ↓
tests
      ↓
target/metadata verification
      ↓
validated release
```

An unresolved hardware conflict must stop the update.

## Project layout

```
targets/    F19 target implementation
tests/      UBYTE-specific tests
docs/       project and update documentation
hardware/   verified schematics/PCB/CAD
tools/      UBYTE firmware manager
```

## Upstream

Official project:
https://github.com/flipperdevices/flipperzero-firmware
