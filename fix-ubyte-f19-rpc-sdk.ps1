$ErrorActionPreference = "Stop"

Set-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)

function Replace-Exact([string]$Path, [string]$Old, [string]$New, [string]$Label) {
    $text = [System.IO.File]::ReadAllText($Path)
    if (-not $text.Contains($Old)) {
        if ($text.Contains($New)) {
            Write-Host "$Label already applied."
            return
        }
        throw "Could not find expected text for $Label in $Path"
    }
    $text = $text.Replace($Old, $New)
    [System.IO.File]::WriteAllText($Path, $text, [System.Text.UTF8Encoding]::new($false))
    Write-Host "Applied: $Label"
}

# 1) Make F19 use its own SDK cache and put its overlay headers before inherited F7 headers.
$target = Join-Path $PWD "targets\f19\target.json"
Replace-Exact $target @'
    "include_paths": [
        "furi_hal"
    ],
'@ @'
    "include_paths": [
        "furi_hal"
    ],
    "sdk_header_paths": [
        "../furi_hal_include",
        "furi_hal",
        "platform_specific"
    ],
    "sdk_symbols": "api_symbols.csv",
'@ "F19 SDK/header selection"

# 2) Restore the UBYTE external GPIO declarations removed by the earlier SDK-header cleanup.
$res_h = Join-Path $PWD "targets\f19\furi_hal\furi_hal_resources.h"
$gpio_decls = @'
/* UBYTE expansion-header GPIO resources. */
extern const GpioPin gpio_ext_pb8;
extern const GpioPin gpio_ext_pb9;
extern const GpioPin gpio_ext_pa0;
extern const GpioPin gpio_ext_pa1;
extern const GpioPin gpio_ext_pa2;
extern const GpioPin gpio_ext_pa3;
extern const GpioPin gpio_ext_pa4;
extern const GpioPin gpio_ext_pa5;
extern const GpioPin gpio_ext_pa6;
extern const GpioPin gpio_ext_pa7;
extern const GpioPin gpio_ext_pa8;
extern const GpioPin gpio_ext_pa9;
extern const GpioPin gpio_ext_pb7;
extern const GpioPin gpio_ext_pb6;
extern const GpioPin gpio_ext_pb5;
extern const GpioPin gpio_ext_pb4;
extern const GpioPin gpio_ext_pb3;
extern const GpioPin gpio_ext_pa10;
extern const GpioPin gpio_ext_pe4;
extern const GpioPin gpio_ext_pb1;
extern const GpioPin gpio_ext_pb0;
extern const GpioPin gpio_ext_pb2;
extern const GpioPin gpio_ext_pa15;

/* Onboard RGB LED pins. */
extern const GpioPin gpio_rgb_red;
extern const GpioPin gpio_rgb_green;
extern const GpioPin gpio_rgb_blue;

'@
Replace-Exact $res_h `
    "extern const InputPin input_pins[];" `
    ($gpio_decls + "extern const InputPin input_pins[];") `
    "UBYTE GPIO declarations"

# 3) Make the generic RPC GPIO mapping compile against only GPIO resources that exist on UBYTE.
$rpc = Join-Path $PWD "applications\services\rpc\rpc_gpio.c"
$rpc_text = [System.IO.File]::ReadAllText($rpc)

$start = $rpc_text.IndexOf("static const GpioPin* rpc_pin_to_hal_pin")
$end = $rpc_text.IndexOf("static GpioMode rpc_mode_to_hal_mode", $start)

if ($start -lt 0 -or $end -lt 0) {
    throw "Could not locate rpc_pin_to_hal_pin() in $rpc"
}

$new_func = @'
static const GpioPin* rpc_pin_to_hal_pin(PB_Gpio_GpioPin rpc_pin) {
    switch(rpc_pin) {
    case PB_Gpio_GpioPin_PB2:
        return &gpio_ext_pb2;
    case PB_Gpio_GpioPin_PB3:
        return &gpio_ext_pb3;
    case PB_Gpio_GpioPin_PA4:
        return &gpio_ext_pa4;
    case PB_Gpio_GpioPin_PA6:
        return &gpio_ext_pa6;
    case PB_Gpio_GpioPin_PA7:
        return &gpio_ext_pa7;
    default:
        __builtin_unreachable();
    }
}

'@

$rpc_text = $rpc_text.Substring(0, $start) + $new_func + $rpc_text.Substring($end)
[System.IO.File]::WriteAllText($rpc, $rpc_text, [System.Text.UTF8Encoding]::new($false))
Write-Host "Applied: UBYTE RPC GPIO mapping"

Write-Host ""
Write-Host "Next verification:"
Write-Host '  git diff --check'
Write-Host '  python -m unittest discover -s tests\ubyte_target -p "test_*.py"'
Write-Host '  .\fbt TARGET_HW=19'
Write-Host ""
Write-Host "Do not modify targets\f7\api_symbols.csv and do not flash yet."
