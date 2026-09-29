import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CLOCK = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_clock.c"
BUS = ROOT / "targets" / "f19" / "furi_hal" / "furi_hal_bus.c"

class UbyteClockBusTest(unittest.TestCase):
    def test_clock_uses_ubyte_crystal_sources_and_stm32wb_64mhz_sequence(self):
        text = CLOCK.read_text(encoding="utf-8") if CLOCK.exists() else ""
        for needle in (
            "CPU_CLOCK_HSE_HZ   32000000",
            "CPU_CLOCK_PLL_HZ   64000000",
            "LL_RCC_HSE_SetCapacitorTuning(0x26);",
            "LL_RCC_HSE_Enable();",
            "LL_RCC_LSE_Enable();",
            "LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE",
            "LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);",
            "LL_C2_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);",
            "LL_SetSystemCoreClock(CPU_CLOCK_PLL_HZ);",
            "LL_RCC_SetRFWKPClockSource(LL_RCC_RFWKP_CLKSOURCE_LSE);",
        ):
            self.assertIn(needle, text)

    def test_bus_exposes_only_physically_used_gpio_ports_and_required_peripherals(self):
        text = BUS.read_text(encoding="utf-8") if BUS.exists() else ""
        self.assertNotIn("LL_AHB2_GRP1_PERIPH_GPIOC", text)
        self.assertNotIn("LL_AHB2_GRP1_PERIPH_GPIOD", text)
        self.assertNotIn("LL_AHB2_GRP1_PERIPH_GPIOH", text)
        for needle in (
            "LL_AHB2_GRP1_PERIPH_GPIOA",
            "LL_AHB2_GRP1_PERIPH_GPIOB",
            "LL_AHB2_GRP1_PERIPH_GPIOE",
            "LL_APB2_GRP1_PERIPH_SPI1",
            "LL_APB2_GRP1_PERIPH_USART1",
            "LL_APB1_GRP1_PERIPH_I2C1",
            "LL_APB1_GRP1_PERIPH_USB",
            "LL_AHB3_GRP1_PERIPH_RNG",
            "LL_AHB3_GRP1_PERIPH_HSEM",
            "LL_AHB3_GRP1_PERIPH_IPCC",
            "LL_APB3_GRP1_PERIPH_RF",
        ):
            self.assertIn(needle, text)

if __name__ == "__main__":
    unittest.main()
