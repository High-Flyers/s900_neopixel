#include <stdio.h>
#include "neopixel.h"

static uint16_t hue = 0;
// config rmt
led_strip_rmt_config_t rmt_conf = {
	.resolution_hz = 10 * 1000 * 1000,
};

// config spi
led_strip_spi_config_t spi_conf = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .spi_bus = SPI2_HOST,           // SPI bus ID
        .flags = {
            .with_dma = true, // Using DMA can improve performance and help drive more LEDs
        }
};

// Konfiguracja paska przekazujac wskaznik do instancji, liczbe ledow i nr pinu
void configure_strip(led_strip_handle_t *strip, uint8_t leds, uint8_t pin){
	led_strip_config_t conf = {
		.max_leds = leds,
		.strip_gpio_num = pin,
	};
	
	led_strip_new_rmt_device(&conf, &rmt_conf, strip);
	//led_strip_new_spi_device(&conf, &spi_conf, strip);
	led_strip_clear(*strip);
}

// Biale podswietlenie o zadanej jasnosci
void backlight(led_strip_handle_t *strip, uint8_t brightness){
	for(int i = 0; i < LED_NUM ; i++)
		led_strip_set_pixel(*strip, i, brightness, brightness, brightness);
	
	led_strip_refresh(*strip);
}

// Przeskok ze zmiana koloru
void animate(led_strip_handle_t *strip)
{
    static uint8_t pixNum = 0;
    static uint8_t state = 0;
    
    //printf("STATE:%d\n", state);
    
    switch (state) {
        case 0: // pokoloruj po kolei ledy
        	
        	// Ze wzgledu na strip delete trzeba pokolorowac wczesniejsze piksele (normalnie bez for)
        	for(int i = 0; i < pixNum; i++)
        		led_strip_set_pixel_hsv(*strip, i, hue, 255, 255);
        	led_strip_refresh(*strip);

            if (pixNum == LED_NUM - 1) {
                state++;
                pixNum = 0;
                break;
            }

            pixNum++;
            break;
            
         case 1: // wygas wszystkie ledy poza ringami
         	
         	// Konieczne pokolorowanie pozostalych pikseli 
         	for(int i = 0; i < LED_NUM; i++)
        		led_strip_set_pixel_hsv(*strip, i, hue, 255, 255);
        	
         	led_strip_set_pixel_hsv(*strip, pixNum, 0,0,0);
        	led_strip_refresh(*strip);
        	
        	if (pixNum == LED_NUM - 1 - 8) {
                state++;
                pixNum = 0;
                break;
            }
            pixNum++;
            break;
            
         case 2: // pokoloruj reszte poza ringami
         	
         	// Konieczne pokolorowanie pozostalych pikseli 
         	for(int i = LED_NUM-8; i < LED_NUM; i++)
        		led_strip_set_pixel_hsv(*strip, i, hue, 255, 255);

        	
         	led_strip_set_pixel_hsv(*strip, LED_NUM - 1 - 8 - pixNum, hue, 255, 255);
        	led_strip_refresh(*strip);
        	
        	if (pixNum == LED_NUM - 1 - 8) {
                state++;
                pixNum = 0;
                break;
            }

            pixNum++;
            break;
            
         case 3: // Wygas wszystkie ledy
         	
         	led_strip_set_pixel_hsv(*strip, LED_NUM - 1 - pixNum, 0,0, 0);
        	led_strip_refresh(*strip);
        	
        	if (pixNum == LED_NUM - 1) {
                state++;
                pixNum = 0;
                break;
            }

            pixNum++;
        	break;
        
        case 4: // clear
           
			vTaskDelay(pdMS_TO_TICKS(300));
			led_strip_clear(*strip);
			led_strip_refresh(*strip);       
            state++;
            break;
        
        case 5: // Zmiana koloru
        
            vTaskDelay(pdMS_TO_TICKS(300));
            state = 0; 
            hue = (hue + HUE_BUMP) % 360;
			printf("HUE:%d\n", hue);
            break;
	}
}