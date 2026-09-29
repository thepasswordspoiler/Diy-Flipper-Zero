# Architecture

The project is organized around three layers:

1. Official Flipper Zero upstream source.
2. UBYTE/F19 target and HAL adaptations.
3. A future update/port manager that fetches upstream changes, reapplies the UBYTE patch stack, builds target 19, runs tests, and stops on conflicts.

The update manager must never silently resolve a hardware conflict or fall back to an official target image.
