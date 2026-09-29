from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
RESOURCES_H = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_resources.h"
RESOURCES_C = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_resources.c"


class UbyteResourcesMappingTest(unittest.TestCase):
    def setUp(self):
        self.header = RESOURCES_H.read_text() if RESOURCES_H.is_file() else ""
        self.source = RESOURCES_C.read_text() if RESOURCES_C.is_file() else ""

    def assert_pin(self, symbol, port, pin):
        pattern = rf"const\s+GpioPin\s+{re.escape(symbol)}\s*=\s*\{{\.port\s*=\s*{port},\s*\.pin\s*=\s*LL_GPIO_PIN_{pin}\s*\}};"
        self.assertRegex(self.source, pattern, f"{symbol} must map to {port}/PIN_{pin}")

    def test_verified_core_mappings(self):
        self.assert_pin("gpio_swdio", "GPIOA", 13)
        self.assert_pin("gpio_swclk", "GPIOA", 14)
        self.assert_pin("gpio_usb_dm", "GPIOA", 11)
        self.assert_pin("gpio_usb_dp", "GPIOA", 12)
        self.assert_pin("gpio_usart_tx", "GPIOA", 9)
        self.assert_pin("gpio_usart_rx", "GPIOA", 10)
        self.assert_pin("gpio_i2c_power_scl", "GPIOB", 8)
        self.assert_pin("gpio_i2c_power_sda", "GPIOB", 9)

    def test_external_headers_contain_only_bonded_ubyte_gpio(self):
        forbidden = ["GPIOC", "GPIOD", "GPIOH"]
        for token in forbidden:
            self.assertNotIn(token, self.source, f"UBYTE resources must not reference {token}")

        gpioe_references = re.findall(r"\.port\s*=\s*GPIOE", self.source)
        self.assertLessEqual(len(gpioe_references), 1, "only the bonded PE4 resource may use GPIOE")

        required_symbols = [
            "gpio_ext_pa0",
            "gpio_ext_pa1",
            "gpio_ext_pa2",
            "gpio_ext_pa3",
            "gpio_ext_pa4",
            "gpio_ext_pa5",
            "gpio_ext_pa6",
            "gpio_ext_pa7",
            "gpio_ext_pa8",
            "gpio_ext_pa9",
            "gpio_ext_pa10",
            "gpio_ext_pa15",
            "gpio_ext_pb0",
            "gpio_ext_pb1",
            "gpio_ext_pb2",
            "gpio_ext_pb3",
            "gpio_ext_pb4",
            "gpio_ext_pb5",
            "gpio_ext_pb6",
            "gpio_ext_pb7",
            "gpio_ext_pb8",
            "gpio_ext_pb9",
            "gpio_ext_pe4",
        ]
        for symbol in required_symbols:
            self.assertIn(symbol, self.source)

        # The UBYTE header exposes PE4 physically, but PE4 is a real bonded pin
        # on this package and is intentionally allowed. The forbidden-port rule
        # above only covers unbonded GPIOC/D/E/H *resources*, so validate the
        # one bonded special-case explicitly.
        self.assertRegex(
            self.source,
            r"const\s+GpioPin\s+gpio_ext_pe4\s*=\s*\{\.port\s*=\s*GPIOE,\s*\.pin\s*=\s*LL_GPIO_PIN_4\s*\};",
        )

    def test_no_f7_only_resource_names_are_declared(self):
        forbidden_names = [
            "gpio_button_up",
            "gpio_button_down",
            "gpio_button_left",
            "gpio_button_right",
            "gpio_button_ok",
            "gpio_button_back",
            "gpio_sdcard_cs",
            "gpio_sdcard_cd",
            "gpio_subghz_cs",
            "gpio_nfc_cs",
            "gpio_rfid_carrier",
            "gpio_rfid_data_in",
            "gpio_rfid_carrier_out",
            "gpio_speaker",
            "gpio_vibro",
        ]
        for symbol in forbidden_names:
            self.assertNotIn(f"{symbol}", self.header)
            self.assertNotIn(f"{symbol}", self.source)


if __name__ == "__main__":
    unittest.main()
