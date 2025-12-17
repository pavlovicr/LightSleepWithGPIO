#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>  
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_timer.h" 
#include "driver/uart.h" 
#include "driver/rtc_io.h"

#define WAKEUP_PIN 12

void app_main(void)
{
    // 1. INICIALIZACIJA
    rtc_gpio_init(WAKEUP_PIN);
    rtc_gpio_set_direction(WAKEUP_PIN, RTC_GPIO_MODE_INPUT_ONLY);
    
    // 2. PULL-UP/PULL-DOWN (PREPREČEVANJE LEBDENJA)
    rtc_gpio_pullup_en(WAKEUP_PIN);      // Vklop pull-up
    rtc_gpio_pulldown_dis(WAKEUP_PIN);   // Izklop pull-down
    // Pin je zdaj stabilen: HIGH ko stikalo ni pritisnjeno
    
    // 3. WAKEUP NA LOW LEVEL (OB PRITISKU)
    rtc_gpio_wakeup_enable(WAKEUP_PIN, GPIO_INTR_LOW_LEVEL);
    
    // 4. PREVERI ZAČETNO STANJE
    printf("Začetno stanje pina: %s\n", 
           rtc_gpio_get_level(WAKEUP_PIN) ? "HIGH" : "LOW");
    
    // Če je stikalo že pritisnjeno, počakaj
    if (rtc_gpio_get_level(WAKEUP_PIN) == 0) {
        printf("Stikalo je pritisnjeno! Sprostite za nadaljevanje...\n");
        while (rtc_gpio_get_level(WAKEUP_PIN) == 0) {
            vTaskDelay(10);
        }
    }
    
    // 5. OMRIKOVANJE (če potrebujemo)
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // 6. VKLOPI WAKEUP
    esp_sleep_enable_gpio_wakeup();
    esp_sleep_enable_timer_wakeup(15000000);
    
    printf("Grem v sleep...\n");
    fflush(stdout);
    
    // 7. SLEEP
    int64_t start = esp_timer_get_time();
    esp_light_sleep_start();
    int64_t end = esp_timer_get_time();
    
    printf("Zbudil sem se po %lld ms\n", (end - start) / 1000);
}