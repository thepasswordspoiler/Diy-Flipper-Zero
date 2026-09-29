import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
FLASH_LD = ROOT / "targets" / "f19" / "stm32wb55xx_flash.ld"
RAM_LD = ROOT / "targets" / "f19" / "stm32wb55xx_ram_fw.ld"
STARTUP_C = ROOT / "targets" / "f19" / "src" / "stm32wb55_startup.c"

class UbyteMemoryLayoutTests(unittest.TestCase):
    def test_flash_linker_declares_expected_regions_and_shared_memory(self):
        text = FLASH_LD.read_text(encoding="utf-8") if FLASH_LD.exists() else ""
        self.assertRegex(text, r"FLASH\s*\(rx\)\s*:\s*ORIGIN\s*=\s*0x08000000\s*,\s*LENGTH\s*=\s*1024K")
        self.assertRegex(text, r"RAM1\s*\(xrw\)\s*:\s*ORIGIN\s*=\s*0x20000008\s*,\s*LENGTH\s*=\s*0x2FFF8")
        self.assertRegex(text, r"RAM2A\s*\(xrw\)\s*:\s*ORIGIN\s*=\s*0x20030000\s*,\s*LENGTH\s*=\s*10K")
        self.assertRegex(text, r"RAM2B\s*\(xrw\)\s*:\s*ORIGIN\s*=\s*0x20038000\s*,\s*LENGTH\s*=\s*10K")
        for section in ("MAPPING_TABLE", "MB_MEM1", "MB_MEM2"):
            self.assertIn(section, text)
        self.assertIn("_stack_end", text)

    def test_ram_linker_declares_full_primary_sram_and_shared_memory(self):
        text = RAM_LD.read_text(encoding="utf-8") if RAM_LD.exists() else ""
        self.assertRegex(text, r"FLASH\s*\(rx\)\s*:\s*ORIGIN\s*=\s*0x08000000\s*,\s*LENGTH\s*=\s*1024K")
        self.assertRegex(text, r"RAM1\s*\(xrw\)\s*:\s*ORIGIN\s*=\s*0x20000000\s*,\s*LENGTH\s*=\s*0x30000")
        self.assertRegex(text, r"RAM2A\s*\(xrw\)\s*:\s*ORIGIN\s*=\s*0x20030000\s*,\s*LENGTH\s*=\s*10K")
        self.assertRegex(text, r"RAM2B\s*\(xrw\)\s*:\s*ORIGIN\s*=\s*0x20038000\s*,\s*LENGTH\s*=\s*10K")
        for section in ("MAPPING_TABLE", "MB_MEM1", "MB_MEM2"):
            self.assertIn(section, text)
        self.assertIn("_stack_end", text)

    def test_startup_preserves_stm32wb_boot_sequence(self):
        text = STARTUP_C.read_text(encoding="utf-8") if STARTUP_C.exists() else ""
        required = [
            "uint32_t SystemCoreClock = 4000000UL;",
            "void SystemInit(void)",
            "RCC->CR &= (uint32_t)0xFAF6FEFBU;",
            "memcpy((void*)&_sdata",
            "memset((void*)&_sbss",
            "memset((void*)&_sMB_MEM2",
            "__libc_init_array();",
            "main();",
            "PLACE_IN_SECTION(\".isr_vector\")",
        ]
        for needle in required:
            self.assertIn(needle, text)

if __name__ == "__main__":
    unittest.main()
