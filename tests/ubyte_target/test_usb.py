import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
USB = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_usb.c"
CDC = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_usb_cdc.c"

class UbyteUsbTest(unittest.TestCase):
    def test_usb_low_level_uses_pa11_pa12_as_af10(self):
        text = USB.read_text(encoding="utf-8") if USB.exists() else ""
        self.assertIn("GPIO_InitStruct.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12;", text)
        self.assertIn("GPIO_InitStruct.Alternate = LL_GPIO_AF_10;", text)
        self.assertIn("LL_GPIO_Init(GPIOA, &GPIO_InitStruct);", text)
        self.assertNotIn("GPIO_InitStruct.Pin = LL_GPIO_PIN_10", text)

    def test_cdc_descriptor_keeps_qflipper_vcp_identity(self):
        text = CDC.read_text(encoding="utf-8") if CDC.exists() else ""
        self.assertIn('USB_STRING_DESC("Flipper Devices Inc.")', text)
        self.assertRegex(text, r"\.idVendor\s*=\s*0x0483")
        self.assertRegex(text, r"\.idProduct\s*=\s*0x5740")
        self.assertIn("usb_cdc_single", text)
        self.assertIn("FuriHalUsbInterface", text)

    def test_cdc_keeps_shared_flipper_cdc_api(self):
        text = CDC.read_text(encoding="utf-8") if CDC.exists() else ""
        for symbol in (
            "furi_hal_cdc_set_callbacks",
            "furi_hal_cdc_get_port_settings",
            "furi_hal_cdc_get_ctrl_line_state",
            "furi_hal_cdc_send",
            "furi_hal_cdc_receive",
        ):
            self.assertIn(symbol, text)

if __name__ == "__main__":
    unittest.main()
