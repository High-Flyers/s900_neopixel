#include "pwm_reader.h"
#include "esp_timer.h"

volatile uint8_t _pin = PWM_IN;
volatile uint8_t _flag = false;

volatile uint16_t _val = 0;

volatile uint64_t _currentMicros = 0;
volatile uint64_t _pulseStartMicros = 0;

void IRAM_ATTR isrRising(void *args);
void IRAM_ATTR isrFalling(void *args);

// Wykrycie początku stanu wysokiego
void IRAM_ATTR isrRising(void *args) {
    _currentMicros = esp_timer_get_time();
    _pulseStartMicros = _currentMicros;

    // Zmiana przerwania na zbocze opadajace
    gpio_isr_handler_remove(PWM_IN);
    gpio_set_intr_type(PWM_IN, GPIO_INTR_NEGEDGE);
    gpio_isr_handler_add(PWM_IN, isrFalling, NULL);
}

// Koniec stanu wysokiego
void IRAM_ATTR isrFalling(void *args) {
    _currentMicros = esp_timer_get_time();
    _val = _currentMicros - _pulseStartMicros;

    // Powrot do poczatkowego przerwania
    gpio_isr_handler_remove(PWM_IN);
    gpio_set_intr_type(PWM_IN, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(PWM_IN, isrRising, NULL);

    if (_val > 1500 && _val < 2100) {
        _flag = true;
    }
}

void pwm_enable() {
    _flag = false;
    
    // Config ze zboczem narastajacym
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << PWM_IN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    
    gpio_config(&io_conf);
    
    gpio_install_isr_service(0);  // Instalujemy ISR (raz na program)
    gpio_isr_handler_add(PWM_IN, isrRising, NULL);
}

// Wylaczenie odczytywania
void pwm_disable() {
    _flag = false;
    gpio_isr_handler_remove(PWM_IN);
}

// Sprawdzenie czy PWM powyzej 1500ms
bool pwm_checkTrigger() {
    return _flag;
}

// Sprawdzenie czy PWM wystepuje w zakresie 1000 - 2000 ms
bool pwm_isAvailable() {
    if ((esp_timer_get_time() - _currentMicros) < 100000 && _val > 950 && _val < 2050) {
        return true;
    } else
        return false;
}

// Wypelnienie PWMa
uint16_t pwm_getUs() {
    return _val;
}
