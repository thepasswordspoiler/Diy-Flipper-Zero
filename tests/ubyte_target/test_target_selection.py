from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]


class UbyteTargetSelectionTest(unittest.TestCase):
    def test_target_19_is_allowed_and_has_explicit_target_definition(self):
        commandline = (ROOT / "site_scons" / "commandline.scons").read_text()
        allowed_block = re.search(
            r'"TARGET_HW".*?allowed_values=\[(.*?)\]', commandline, re.S
        )
        self.assertIsNotNone(allowed_block, "TARGET_HW allowed_values block not found")
        allowed_values = allowed_block.group(1)
        self.assertIn('"19"', allowed_values)

        target_file = ROOT / "targets" / "f19" / "target.json"
        self.assertTrue(target_file.is_file(), "targets/f19/target.json is missing")


if __name__ == "__main__":
    unittest.main()
