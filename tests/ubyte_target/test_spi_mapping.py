import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
RESOURCES = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_resources.c"
SPI_CONFIG = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_spi_config.c"
SPI_HEADER = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_spi_config.h"

class UbyteSpiMappingTest(unittest.TestCase):
    def test_selected_spi1_pins_and_af_are_ubyte_safe(self):
        resources = RESOURCES.read_text(encoding="utf-8") if RESOURCES.exists() else ""
        spi = SPI_CONFIG.read_text(encoding="utf-8") if SPI_CONFIG.exists() else ""
        header = SPI_HEADER.read_text(encoding="utf-8") if SPI_HEADER.exists() else ""

        self.assertRegex(resources, r"gpio_spi_d_sck\s*=\s*\{\.port\s*=\s*GPIOA,\s*\.pin\s*=\s*LL_GPIO_PIN_5\}")
        self.assertRegex(resources, r"gpio_spi_d_miso\s*=\s*\{\.port\s*=\s*GPIOA,\s*\.pin\s*=\s*LL_GPIO_PIN_6\}")
        self.assertRegex(resources, r"gpio_spi_d_mosi\s*=\s*\{\.port\s*=\s*GPIOA,\s*\.pin\s*=\s*LL_GPIO_PIN_7\}")
        self.assertRegex(resources, r"gpio_display_cs\s*=\s*\{\.port\s*=\s*GPIOA,\s*\.pin\s*=\s*LL_GPIO_PIN_4\}")

        for pin in ("gpio_spi_d_sck", "gpio_spi_d_miso", "gpio_spi_d_mosi"):
            self.assertIn(pin, spi)
        self.assertGreaterEqual(spi.count("GpioAltFn5SPI1"), 3)
        self.assertIn("furi_hal_spi_bus_handle_external", spi)
        self.assertIn("furi_hal_spi_bus_handle_external", header)

        for rgb_pin in ("LL_GPIO_PIN_15", "LL_GPIO_PIN_4", "LL_GPIO_PIN_5"):
            if rgb_pin == "LL_GPIO_PIN_4":
                # PA4 is allowed as software CS; only the RGB PB4/PA15/PB5 pins are forbidden.
                self.assertNotIn("gpio_display_cs = {.port = GPIOB", resources)
            elif rgb_pin == "LL_GPIO_PIN_5":
                self.assertNotRegex(resources, r"gpio_spi_(?:sck|miso|mosi)\s*=\s*\{\.port\s*=\s*GPIOB,\s*\.pin\s*=\s*LL_GPIO_PIN_5\}")
            else:
                self.assertNotRegex(resources, r"gpio_spi_(?:sck|miso|mosi|cs)\s*=\s*\{\.port\s*=\s*GPIOA,\s*\.pin\s*=\s*LL_GPIO_PIN_15\}")

if __name__ == "__main__":
    unittest.main()
