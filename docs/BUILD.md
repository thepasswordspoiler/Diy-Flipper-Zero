# Build

The known-good development tree uses the Flipper build system (fbt) with target F19.

Before releasing a build:

1. Build target F19.
2. Run the UBYTE target tests.
3. Verify generated target metadata reports target 19.
4. Verify qFlipper identifies the device as the UBYTE target.
5. Keep the resulting source commit and artifact hashes together.
