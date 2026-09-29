import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
IDENTITY = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_version_device.c"

class UbyteDeviceIdentityTest(unittest.TestCase):
    def test_identity_is_scoped_to_ubyte_target_and_not_f7_regulatory_ids(self):
        text = IDENTITY.read_text(encoding="utf-8") if IDENTITY.exists() else ""
        self.assertIn("furi_hal_version_get_hw_target() == 19", text)
        self.assertIn('return "UBYTE STM32WB55";', text)
        self.assertIn('return "UBYTE.1";', text)
        for forbidden in ("2A2V6-FZ", "27624-FZ", "210-175991", "2023DJ16420", "CCAJ23LP34D0T3"):
            self.assertNotIn(forbidden, text)

    def test_identity_does_not_use_target_7_as_ownership_condition(self):
        text = IDENTITY.read_text(encoding="utf-8") if IDENTITY.exists() else ""
        self.assertNotIn("== 7", text)
        self.assertNotIn("|| furi_hal_version_get_hw_target() == 7", text)

if __name__ == "__main__":
    unittest.main()
