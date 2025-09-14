#include <stdint.h>
#include "motor_pwm.h"

// ======= MCPWM0 BASE ADDRESS (check your actual SoC header for ESP32-S3) =======
#define MCPWM0_BASE                  0x3F400000

// ======= MCPWM REGISTER OFFSETS =======
#define MCPWM_CLK_CFG_REG            (MCPWM0_BASE + 0x0000)
#define MCPWM_TIMER0_CFG0_REG        (MCPWM0_BASE + 0x0004)
#define MCPWM_TIMER0_CFG1_REG        (MCPWM0_BASE + 0x0008)
#define MCPWM_GEN0_CFG0_REG          (MCPWM0_BASE + 0x0048)
#define MCPWM_GEN0_A_REG             (MCPWM0_BASE + 0x0050)
#define MCPWM_UPDATE_CFG_REG         (MCPWM0_BASE + 0x010C)

// ======= GPIO MATRIX AND GPIO BASE ADDRESSES =======
#define DR_REG_GPIO_BASE             0x3F404000
#define DR_REG_GPIO_MATRIX_BASE      0x3F408000

// GPIO18 matrix output config register
#define GPIO_FUNC18_OUT_SEL_CFG_REG  (DR_REG_GPIO_MATRIX_BASE + 0x0118)
#define GPIO_ENABLE_REG              (DR_REG_GPIO_BASE + 0x0020)    // Output enable

// ======= GPIO MATRIX SIGNAL INDEX FOR MCPWM0A (see ESP32-S3 datasheet) =======
#define MCPWM0A_OUT_IDX              0    // Usually 0 for MCPWM0 PWM0A

// ======= BIT FIELD MASKS AND SHIFTS =======
// MCPWM_CLK_CFG_REG (prescaler)
#define MCPWM_CLK_PRESCALE_M         0xFF
#define MCPWM_CLK_PRESCALE_S         0

// MCPWM_TIMERx_CFG0_REG (period)
#define MCPWM_TIMER_PERIOD_M         0xFFFF
#define MCPWM_TIMER_PERIOD_S         0

// MCPWM_TIMERx_CFG1_REG (start/stop)
#define MCPWM_TIMER_START_M          0x3
#define MCPWM_TIMER_START_S          0

// MCPWM_UPDATE_CFG_REG
#define MCPWM_TIMER0_UPDATE_EN_M     (1<<0)
#define MCPWM_GEN0_UPDATE_EN_M       (1<<4)

// ======= SIMPLE REGISTER MACROS =======
#define REG_WRITE(addr, val)         (*(volatile uint32_t *)(addr) = (val))
#define REG_READ(addr)               (*(volatile uint32_t *)(addr))

/**
 * @brief Initialize MCPWM0 PWM0A to output 20kHz 50% duty cycle on GPIO18.
 * @details Configures MCPWM0 timer for 20kHz (160MHz / 8000 ticks), sets 50% duty (4000 ticks),
 *          and routes PWM0A to GPIO18 via the GPIO matrix.
 */
void mcpwm0_pwm0a_gpio18_20khz_50pct(void)
{
    // 1. Set MCPWM prescaler to 0 (no division), so timer runs at 160MHz APB clock
    REG_WRITE(MCPWM_CLK_CFG_REG, (0 << MCPWM_CLK_PRESCALE_S));

    // 2. Set PWM period for 20 kHz
    // Timer freq = 160MHz, period = 160MHz / 20kHz = 8000 ticks
    // Period register is zero-based, so use 7999
    REG_WRITE(MCPWM_TIMER0_CFG0_REG, (7999 << MCPWM_TIMER_PERIOD_S));

    // 3. Set duty (compare value) for 50% (4000 ticks)
    REG_WRITE(MCPWM_GEN0_A_REG, 4000);

    // 4. Generator config: use default PWM mode (output high between 0 and compare, low otherwise)
    REG_WRITE(MCPWM_GEN0_CFG0_REG, 0x0);

    // 5. Enable update for timer 0 and generator 0
    REG_WRITE(MCPWM_UPDATE_CFG_REG, MCPWM_TIMER0_UPDATE_EN_M | MCPWM_GEN0_UPDATE_EN_M);

    // 6. Start timer 0 (write 2'b01 to start bits in MCPWM_TIMER0_CFG1_REG)
    REG_WRITE(MCPWM_TIMER0_CFG1_REG, (1 << MCPWM_TIMER_START_S));

    // 7. Route MCPWM0 PWM0A to GPIO18 via GPIO Matrix
    REG_WRITE(GPIO_FUNC18_OUT_SEL_CFG_REG, MCPWM0A_OUT_IDX);

    // 8. Enable GPIO18 as output
    REG_WRITE(GPIO_ENABLE_REG, REG_READ(GPIO_ENABLE_REG) | (1 << 18));
}

/**
 * @brief Set PWM0A duty cycle in ticks.
 * @param duty_ticks Number of ticks (0-8000 for 0-100% duty at 20kHz).
 * @details Updates the PWM0A compare value and triggers an update to apply the new duty cycle.
 */
void mcpwm0_pwm0a_set_duty(uint16_t duty_ticks)
{
    // Ensure duty_ticks does not exceed period (8000 ticks)
    if (duty_ticks > 8000) {
        duty_ticks = 8000;
    }
    // Update compare value for PWM0A
    REG_WRITE(MCPWM_GEN0_A_REG, duty_ticks);
    // Trigger update to apply new duty cycle
    REG_WRITE(MCPWM_UPDATE_CFG_REG, MCPWM_GEN0_UPDATE_EN_M);
}
