#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

#define SYSTEM_REG_BASE       0x600C0000
#define SYSTEM_SYSCLK_CONF_REG_OFFSET    0x0060
#define SYSTEM_CPU_PER_CONF_REG_OFFSET   0x0010

#define SYSTEM_SOC_CLK_SEL_SHIFT         27
#define SYSTEM_SOC_CLK_SEL_MASK          (0x3 << SYSTEM_SOC_CLK_SEL_SHIFT)

#define SYSTEM_CPUPERIOD_SEL_SHIFT       0
#define SYSTEM_CPUPERIOD_SEL_MASK        (0x3 << SYSTEM_CPUPERIOD_SEL_SHIFT)
#define SYSTEM_PLL_FREQ_SEL_SHIFT        2
#define SYSTEM_PLL_FREQ_SEL_MASK         (0x1 << SYSTEM_PLL_FREQ_SEL_SHIFT)

#define REG32(addr)    (*(volatile uint32_t *)(addr))

void system_set_cpu_clock_240mhz(void);

#endif /* SYSTEM_H */
