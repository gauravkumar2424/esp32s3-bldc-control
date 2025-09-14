#include "state_machine.h"
#include "motor_pwm.h"
#include "pid_controller.h"
#include "esp_log.h"

static const char *TAG = "STATE_MACHINE";

/**
 * @brief Initialize state machine to INIT state.
 * @param sm Pointer to state machine context.
 */
void state_machine_init(state_machine_t *sm) {
    sm->current_state = STATE_INIT;
}

/**
 * @brief Handle external state requests.
 * @param sm Pointer to state machine context.
 * @param requested Requested state to transition to.
 * @details Allows transitions: INIT -> RUN, STOP -> RUN, RUN -> STOP, any -> FAULT, any -> INIT.
 */
void state_machine_request(state_machine_t *sm, motor_state_t requested) {
    switch (requested) {
        case STATE_RUN:
            if (sm->current_state == STATE_INIT || sm->current_state == STATE_STOP) {
                sm->current_state = STATE_RUN;
                ESP_LOGI(TAG, "Transitioned to RUN");
            }
            break;
        case STATE_STOP:
            if (sm->current_state == STATE_RUN) {
                sm->current_state = STATE_STOP;
                ESP_LOGI(TAG, "Transitioned to STOP");
            }
            break;
        case STATE_FAULT:
            sm->current_state = STATE_FAULT;
            ESP_LOGE(TAG, "Transitioned to FAULT");
            break;
        case STATE_INIT:
            sm->current_state = STATE_INIT;
            ESP_LOGI(TAG, "Transitioned to INIT");
            break;
        default:
            ESP_LOGW(TAG, "Invalid state request: %d", requested);
            break;
    }
}

/**
 * @brief Run one step of the state machine.
 * @param sm Pointer to state machine context.
 * @param pid Pointer to PID controller (for RUN state).
 * @param setpoint Target speed for PID (in Hz).
 * @param measured_speed Current measured speed (in Hz).
 * @details Executes state-specific actions, including PWM control.
 */
void state_machine_step(state_machine_t *sm, pid_controller_t *pid, float setpoint, float measured_speed) {
    switch (sm->current_state) {
        case STATE_INIT:
            // Initialize PWM with 50% duty cycle
            mcpwm0_pwm0a_gpio18_20khz_50pct();
            pid_reset(pid); // Reset PID controller
            ESP_LOGI(TAG, "Initialized PWM and PID");
            // Automatically transition to RUN after initialization
            state_machine_request(sm, STATE_RUN);
            break;
        case STATE_STOP:
            // Set PWM duty to 0 (stop motor)
            mcpwm0_pwm0a_set_duty(0);
            ESP_LOGI(TAG, "Motor stopped");
            break;
        case STATE_RUN:
            // Update PWM duty based on PID output
            float duty = pid_update(pid, setpoint, measured_speed);
            mcpwm0_pwm0a_set_duty((uint16_t)(duty * 80.0f)); // Convert 0-100% to 0-8000 ticks
            ESP_LOGI(TAG, "Running: Speed=%.2f Hz, Duty=%.2f%%", measured_speed, duty);
            // Basic fault detection: check if speed is unrealistic
            if (measured_speed > 2000.0f || measured_speed < 0.0f) {
                state_machine_request(sm, STATE_FAULT);
            }
            break;
        case STATE_FAULT:
            // Disable PWM and log error
            mcpwm0_pwm0a_set_duty(0);
            ESP_LOGE(TAG, "Fault: Speed=%.2f Hz", measured_speed);
            break;
    }
}
