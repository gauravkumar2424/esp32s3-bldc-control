#include "pid_controller.h"

// Initialize PID struct
void pid_init(pid_controller_t *pid, float Kp, float Ki, float Kd, float dt, float out_min, float out_max) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->dt = dt;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->out_min = out_min;
    pid->out_max = out_max;
}

// Reset integral and prev error
void pid_reset(pid_controller_t *pid) {
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

// Compute PID output (call every dt seconds)
float pid_update(pid_controller_t *pid, float setpoint, float measurement) {
    float error = setpoint - measurement;
    pid->integral += error * pid->dt;
    float derivative = (error - pid->prev_error) / pid->dt;
    float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    // Anti-windup and clamping
    if (output > pid->out_max) {
        output = pid->out_max;
        // Optionally prevent further integral windup:
        // pid->integral -= error * pid->dt;
    } else if (output < pid->out_min) {
        output = pid->out_min;
        // pid->integral -= error * pid->dt;
    }

    pid->prev_error = error;
    return output;
}
