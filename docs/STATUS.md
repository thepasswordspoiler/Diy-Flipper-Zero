# Project Status

## Known-good checkpoint

| Item | Value |
|---|---|
| Hardware | UBYTE STM32WB55 |
| Target | F19 / 19 |
| Firmware | 0.1.1 |
| API | 91.0 |
| Protobuf | 0.25 |
| USB VCP | 0483:5740 |
| Device name | UBYTE-F1 |
| qFlipper | Connected / RPC / screen stream working |
| USB DFU | Working |
| Factory keys | Unavailable / intentionally not implemented |
| SD card | Unavailable / not implemented |

## Safety rule

Official Flipper Zero firmware must not be flashed directly to the UBYTE board. Future updates must be rebased/reconciled into the F19 port and validated before flashing.
