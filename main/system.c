#include "system.h"

void system_set_cpu_clock_240mhz(void) {
    // Set PLL_FREQ_SEL = 1 (480 MHz PLL)
    // Set CPUPERIOD_SEL = 2 (PLL/2 = 240 MHz)
    uint32_t cpu_per_conf = REG32(SYSTEM_REG_BASE + SYSTEM_CPU_PER_CONF_REG_OFFSET);
    cpu_per_conf &= ~(SYSTEM_CPUPERIOD_SEL_MASK | SYSTEM_PLL_FREQ_SEL_MASK);
    cpu_per_conf |= (2 << SYSTEM_CPUPERIOD_SEL_SHIFT); // CPUPERIOD_SEL = 2 (PLL/2)
    cpu_per_conf |= (1 << SYSTEM_PLL_FREQ_SEL_SHIFT);   // PLL_FREQ_SEL = 1 (480MHz PLL)
    REG32(SYSTEM_REG_BASE + SYSTEM_CPU_PER_CONF_REG_OFFSET) = cpu_per_conf;

    uint32_t sysclk_conf = REG32(SYSTEM_REG_BASE + SYSTEM_SYSCLK_CONF_REG_OFFSET);
    sysclk_conf &= ~SYSTEM_SOC_CLK_SEL_MASK;
    sysclk_conf |= (1 << SYSTEM_SOC_CLK_SEL_SHIFT);     // SOC_CLK_SEL = 1 (PLL_CLK)
    REG32(SYSTEM_REG_BASE + SYSTEM_SYSCLK_CONF_REG_OFFSET) = sysclk_conf;

}
