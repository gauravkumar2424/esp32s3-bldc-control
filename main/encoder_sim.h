#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize encoder input on GPIO19 (A) and GPIO21 (B) or simulation mode.
 * @details Configures GPIO interrupts for quadrature encoder signals or initializes simulation if SIMULATE_ENCODER is defined.
 */
void encoder_sim_init(void);

/**
 * @brief Measure encoder speed in Hz.
 * @return Measured speed in Hz based on pulse counts (real or simulated).
 */
float encoder_sim_measure_speed(void);

#ifdef __cplusplus
}
#endif
