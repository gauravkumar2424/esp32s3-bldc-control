#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "pid_controller.h"

typedef enum {
    STATE_INIT = 0,
    STATE_STOP,
    STATE_RUN,
    STATE_FAULT
} motor_state_t;

// State machine context
typedef struct {
    motor_state_t current_state;
} state_machine_t;

/**
 * @brief Initialize the state machine to INIT state.
 * @param sm Pointer to state machine context.
 */
void state_machine_init(state_machine_t *sm);

/**
 * @brief Request a transition to a new state.
 * @param sm Pointer to state machine context.
 * @param requested Requested state to transition to.
 */
void state_machine_request(state_machine_t *sm, motor_state_t requested);

/**
 * @brief Run one step of the state machine.
 * @param sm Pointer to state machine context.
 * @param pid Pointer to PID controller (for RUN state).
 * @param setpoint Target speed for PID (in Hz).
 * @param measured_speed Current measured speed (in Hz).
 */
void state_machine_step(state_machine_t *sm, pid_controller_t *pid, float setpoint, float measured_speed);

#ifdef __cplusplus
}
#endif
