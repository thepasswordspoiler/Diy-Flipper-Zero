import json
from pathlib import Path
import unittest

ROOT = Path(__file__).parents[2]
F19 = ROOT / "targets" / "f19"
F7 = ROOT / "targets" / "f7"


def merged_target(target_id: str):
    target = ROOT / "targets" / f"f{target_id}"
    cfg = json.loads((target / "target.json").read_text())
    parent = cfg.get("inherit")
    if parent:
        base = merged_target(str(parent))
    else:
        base = {}
    out = dict(base)
    for key in ("include_paths", "sdk_header_paths", "excluded_sources", "excluded_headers", "excluded_modules"):
        values = list(out.get(key, []))
        values.extend(cfg.get(key, []))
        out[key] = values
    for key in ("linker_script_flash", "linker_script_ram", "linker_script_app", "sdk_symbols"):
        if key in cfg:
            out[key] = cfg[key]
    if "linker_dependencies" in cfg:
        out["linker_dependencies"] = cfg["linker_dependencies"]
    return out


class TargetCompositionTest(unittest.TestCase):
    def test_f19_replaces_f7_library_and_overlays_board_hal(self):
        cfg = merged_target("19")
        deps = cfg["linker_dependencies"]
        self.assertIn("flipper19", deps)
        self.assertNotIn("flipper7", deps)
        for filename in (
            "furi_hal.c",
            "main.c",
        ):
            self.assertTrue((F19 / ("furi_hal" if filename.startswith("furi_hal") else "src") / filename).exists(), filename)

    def test_f19_excludes_unwired_f7_board_sources(self):
        cfg = merged_target("19")
        for filename in (
            "furi_hal_ibutton.c",
            "furi_hal_infrared.c",
            "furi_hal_nfc.c",
            "furi_hal_nfc_event.c",
            "furi_hal_nfc_felica.c",
            "furi_hal_nfc_irq.c",
            "furi_hal_nfc_iso14443a.c",
            "furi_hal_nfc_iso14443b.c",
            "furi_hal_nfc_iso15693.c",
            "furi_hal_nfc_timer.c",
            "furi_hal_rfid.c",
            "furi_hal_speaker.c",
            "furi_hal_subghz.c",
            "furi_hal_vibro.c",
            "recovery.c",
        ):
            self.assertIn(filename, cfg["excluded_sources"], filename)

    def test_f19_excludes_unwired_modules(self):
        cfg = merged_target("19")
        for module in ("nfc", "lfrfid", "subghz", "ibutton", "infrared"):
            self.assertIn(module, cfg["excluded_modules"], module)

    def test_f7_target_json_and_default_are_untouched(self):
        f7 = json.loads((F7 / "target.json").read_text())
        self.assertNotIn("19", json.dumps(f7))
        commandline = (ROOT / "site_scons" / "commandline.scons").read_text()
        self.assertIn('default="7"', commandline)


if __name__ == "__main__":
    unittest.main()

# The shared GUI service needs a display SPI handle even when no panel is connected yet.
def _text(path):
    return path.read_text()

class GuiCompositionTest(unittest.TestCase):
    def test_ubyte_reserves_external_oled_control_pins_for_shared_gui(self):
        resources = _text(F19 / "furi_hal" / "furi_hal_resources.h")
        source = _text(F19 / "furi_hal" / "furi_hal_resources.c")
        spi_header = _text(F19 / "furi_hal" / "furi_hal_spi_config.h")
        spi_source = _text(F19 / "furi_hal" / "furi_hal_spi_config.c")
        for symbol in ("gpio_display_cs", "gpio_display_di", "gpio_display_rst_n"):
            self.assertIn(symbol, resources)
            self.assertIn(symbol, source)
        self.assertIn("furi_hal_spi_bus_handle_display", spi_header)
        self.assertIn("furi_hal_spi_bus_handle_display", spi_source)
        self.assertNotIn('"gui"', json.loads((F19 / "target.json").read_text()).get("excluded_modules", []))

class TargetLinkerSelectionTest(unittest.TestCase):
    def test_f19_selects_its_local_linker_scripts(self):
        cfg = json.loads((F19 / "target.json").read_text())
        self.assertEqual(cfg.get("linker_script_flash"), "stm32wb55xx_flash.ld")
        self.assertEqual(cfg.get("linker_script_ram"), "stm32wb55xx_ram_fw.ld")
        self.assertEqual(cfg.get("linker_script_app"), "application_ext.ld")
        self.assertTrue((F19 / cfg["linker_script_flash"]).is_file())
        self.assertTrue((F19 / cfg["linker_script_ram"]).is_file())
        self.assertTrue((F19 / cfg["linker_script_app"]).is_file())
