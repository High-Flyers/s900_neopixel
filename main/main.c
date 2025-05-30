#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "pwm_reader.h"
#include "neopixel.h"

led_strip_handle_t *strip [STRIP_NUM];
uint8_t map_pins [STRIP_NUM] = {0, 1, 2, 21, 22, 23};
uint16_t pwm_val = 0;
 
// Odczytywanie wartosci PWM - 500ms
void read_pwm(){
	
	pwm_enable();
	
	for(;;){
		pwm_val = pwm_getUs();
		printf("PWM: %d", pwm_val);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

// Sterowanie LEDami na podstawie sygnalu PWM - 20ms
void led_control(){
	
	// Poczatkowy stan
	for(int i = 0; i < STRIP_NUM; i++) {
		configure_strip(&strip[i], LED_NUM, map_pins[i]);
		backlight(&strip[i], 50);
		led_strip_del(strip[i]);
	}
	
	for(;;){
		// PWM niski - jasny bialy
		if (pwm_val > 800 && pwm_val < 1200) {
			for(int i = 0; i < STRIP_NUM; i++) {
				if(i==2 || i==5){}
				else{
					configure_strip(&strip[i], LED_NUM, map_pins[i]);
					backlight(&strip[i], 255);
					led_strip_del(strip[i]);
				}
			}
			// Boki - zielony blink
			configure_strip(&strip[2], LED_NUM, map_pins[2]);
			blink(&strip[2], 0, 250, 0);
			led_strip_del(strip[2]);
			configure_strip(&strip[5], LED_NUM, map_pins[5]);
			blink(&strip[5], 0, 250, 0);
			led_strip_del(strip[5]);
		}
		// PWM wysoki - animacja
		else if (pwm_val > 1800 && pwm_val < 2200) {
			/*for(int i = 0; i < STRIP_NUM; i++) {
				configure_strip(&strip[i], LED_NUM, map_pins[i]);
				animate(&strip[i]);
				led_strip_del(strip[i]);
			}*/
			// Przod - czerwony
			configure_strip(&strip[0], LED_NUM, map_pins[0]);
			colour(&strip[0], 250, 0, 0);
			led_strip_del(strip[0]);
			configure_strip(&strip[1], LED_NUM, map_pins[1]);
			colour(&strip[1], 250, 0, 0);
			led_strip_del(strip[2]);
			// Boki - zielony blink
			configure_strip(&strip[2], LED_NUM, map_pins[2]);
			blink(&strip[2], 0, 250, 0);
			led_strip_del(strip[2]);
			configure_strip(&strip[5], LED_NUM, map_pins[5]);
			blink(&strip[5], 0, 250, 0);
			led_strip_del(strip[5]);
			// Srodek - zielony
			configure_strip(&strip[3], LED_NUM, map_pins[3]);
			colour(&strip[3], 0, 250, 0);
			led_strip_del(strip[3]);
			configure_strip(&strip[4], LED_NUM, map_pins[4]);
			colour(&strip[4], 0, 250 ,0);
			led_strip_del(strip[4]);
		}
		
		// brak PWM lub inna wartosc - podswietlenie o niskim natezeniu
		else {
			for(int i = 0; i < STRIP_NUM; i++) {
				configure_strip(&strip[i], LED_NUM, map_pins[i]);
				backlight(&strip[i], 10);
				led_strip_del(strip[i]);
			}
		}
		
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void app_main(void)
{
	xTaskCreatePinnedToCore(read_pwm, "pwm", 2048, NULL, 2, NULL, 0);
	xTaskCreatePinnedToCore(led_control, "led", 4096, NULL, 1, NULL, 0);
}
