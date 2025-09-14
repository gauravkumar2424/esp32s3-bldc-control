#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file motor_pwm.h
 * @brief Bare-metal MCPWM0 (PWM0A) driver for ESP32-S3.
 *
 * Provides initialization and control for 20kHz PWM on GPIO18.
 *
 * Usage:
 *   #include "motor_pwm.h"
 *   ...
 *   mcpwm0_pwm0a_gpio18_20khz_50pct();
 *   mcpwm0_pwm0a_set_duty(4000); // Set 50% duty (4000/8000 ticks)
 */

/**
 * @brief Initialize MCPWM0 PWM0A to output 20kHz 50% duty cycle on GPIO18.
 * @details Configures MCPWM0, maps PWM0A to GPIO18, and enables output.
 *          Call once during system initialization.
 */
void mcpwm0_pwm0a_gpio18_20khz_50pct(void);

/**
 * @brief Set PWM0A duty cycle in ticks.
 * @param duty_ticks Number of ticks (0-8000 for 0-100% duty at 20kHz).
 */
void mcpwm0_pwm0a_set_duty(uint16_t duty_ticks);

// Placeholder for future API expansion:
// void mcpwm0_pwm0a_set_frequency(uint32_t freq_hz);

#ifdef __cplusplus
}
#endif
