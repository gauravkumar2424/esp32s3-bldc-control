#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float dt;        // Time step (seconds)
    float prev_error;
    float integral;
    float out_min, out_max; // Output clamping (e.g., 0.0 to 100.0 for PWM duty)
} pid_controller_t;

// Initialize PID controller struct
void pid_init(pid_controller_t *pid, float Kp, float Ki, float Kd, float dt, float out_min, float out_max);

// Reset integral and previous error
void pid_reset(pid_controller_t *pid);

// Compute next PID output
float pid_update(pid_controller_t *pid, float setpoint, float measurement);

#ifdef __cplusplus
}
#endif
