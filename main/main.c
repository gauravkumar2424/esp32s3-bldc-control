#include <stdint.h>
#include "system.h"
#include "motor_pwm.h"
#include "encoder_sim.h"
#include "pid_controller.h"
#include "state_machine.h"
#include "esp_task_wdt.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void) {
    esp_task_wdt_deinit(); // Disable the ESP-IDF task watchdog

    ESP_LOGI(TAG, "Starting motor control loop!");

    // 1. Initialize system clock
    system_set_cpu_clock_240mhz();
    encoder_sim_init();

    // 2. Initialize PID controller
    pid_controller_t pid;
    pid_init(&pid, 0.02f, 0.005f, 0.0f, 0.01f, 0.0f, 100.0f); // Tune Kp, Ki, dt, limits

    // 3. Initialize state machine
    state_machine_t sm;
    state_machine_init(&sm);

    // 4. Request RUN state
    state_machine_request(&sm, STATE_RUN);

    // 5. Target speed (setpoint) in Hz
    float setpoint = 800.0f;

    // 6. Main loop
    while (1) {
        float measured_speed = encoder_sim_measure_speed();
        state_machine_step(&sm, &pid, setpoint, measured_speed);

        // Insert a simple delay (~10ms for 100Hz loop)
        for (volatile int i = 0; i < 100000; ++i) __asm__ volatile ("nop");
    }
}
