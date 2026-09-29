import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
F7 = ROOT / "targets" / "f7" / "furi_hal" / "furi_hal_resources.h"
F19 = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_resources.h"
SPI = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_spi_config.h"


def extern_names(path):
    text = path.read_text(encoding="utf-8")
    return set(
        re.findall(
            r"extern\s+(?:const|static)?\s*[^;\n]*?\b([A-Za-z_][A-Za-z0-9_]*)\s*;",
            text,
        )
    )


class UbyteSdkApiSurfaceTest(unittest.TestCase):
    def test_f19_public_resource_symbols_are_existing_f7_api_symbols(self):
        new_symbols = extern_names(F19) - extern_names(F7)
        self.assertEqual(new_symbols, set())

    def test_f19_spi_bus_reuses_existing_f7_bus_symbol(self):
        text = SPI.read_text(encoding="utf-8")
        self.assertIn("furi_hal_spi_bus_d", text)
        self.assertNotIn("extern FuriHalSpiBus furi_hal_spi_bus;", text)
