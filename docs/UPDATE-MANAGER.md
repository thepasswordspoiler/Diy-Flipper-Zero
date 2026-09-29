# UBYTE Firmware Manager

Planned workflow:

```text
fetch upstream
    -> verify upstream revision
    -> apply UBYTE patch/overlay stack
    -> detect conflicts
    -> build F19
    -> run tests
    -> verify target 19
    -> package artifacts
    -> backup current firmware
    -> flash only validated F19 image
    -> verify USB/RPC/device metadata
```

A conflict must stop the process. The manager must never automatically convert an unresolved UBYTE hardware change into an official Flipper target.
