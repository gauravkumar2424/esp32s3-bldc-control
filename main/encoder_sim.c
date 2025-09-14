#include "encoder_sim.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_timer.h"

static const char *TAG = "ENCODER";

// Define this to enable simulation mode (no real encoder needed)
#define SIMULATE_ENCODER

#ifdef SIMULATE_ENCODER
// Simulated pulse counter and timing
static volatile int32_t sim_pulse_count = 0;
static volatile uint64_t sim_last_time = 0;
#else
// Real encoder GPIO pins
#define ENCODER_PIN_A 19
#define ENCODER_PIN_B 21

// Pulse counter and timing variables for real encoder
static volatile int32_t pulse_count = 0;
static volatile uint64_t last_pulse_time = 0;
#endif

static volatile float measured_speed = 0.0f;

// ISR for encoder pin A (real mode)
#ifndef SIMULATE_ENCODER
static void IRAM_ATTR encoder_isr_handler(void *arg) {
    uint64_t current_time = esp_timer_get_time();
    // Check direction using pin B's state
    int level_b = gpio_get_level(ENCODER_PIN_B);
    if (level_b) {
        pulse_count++; // Forward
    } else {
        pulse_count--; // Reverse
    }
    last_pulse_time = current_time;
}
#endif

/**
 * @brief Initialize encoder input or simulation.
 */
void encoder_sim_init(void) {
#ifndef SIMULATE_ENCODER
    // Configure GPIO pins for real encoder
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << ENCODER_PIN_A) | (1ULL << ENCODER_PIN_B),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE // Trigger on rising edge of pin A
    };
    gpio_config(&io_conf);

    // Install ISR for pin A
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ENCODER_PIN_A, encoder_isr_handler, NULL);
    ESP_LOGI(TAG, "Initialized real encoder on GPIO %d (A), %d (B)", ENCODER_PIN_A, ENCODER_PIN_B);
#else
    // Initialize simulation mode
    sim_last_time = esp_timer_get_time();
    ESP_LOGI(TAG, "Initialized simulated encoder");
#endif
}

/**
 * @brief Measure encoder speed in Hz.
 * @return Measured speed in Hz (real or simulated).
 */
float encoder_sim_measure_speed(void) {
#ifdef SIMULATE_ENCODER
    // Simulate speed (matches previous dummy function)
    static float fake_speed = 400.0f;
    fake_speed += 0.5f;
    if (fake_speed > 1000.0f) fake_speed = 400.0f;
    measured_speed = fake_speed;
    ESP_LOGD(TAG, "Simulated speed: %.2f Hz", measured_speed);
#else
    // Calculate speed based on pulse count
    uint64_t current_time = esp_timer_get_time();
    int32_t pulses = pulse_count;
    pulse_count = 0; // Reset count
    uint64_t time_diff = current_time - last_pulse_time;
    last_pulse_time = current_time;

    // Assume 1000 pulses per revolution (adjust for your encoder)
    // Speed (Hz) = (pulses / pulses_per_rev) / (time_diff / 1e6)
    float pulses_per_rev = 1000.0f;
    if (time_diff > 0) {
        measured_speed = (pulses / pulses_per_rev) * (1000000.0f / time_diff);
    } else {
        measured_speed = 0.0f;
    }
    ESP_LOGD(TAG, "Real speed: %.2f Hz, pulses: %ld, time_diff: %llu us", measured_speed, pulses, time_diff);
#endif
    return measured_speed;
}
