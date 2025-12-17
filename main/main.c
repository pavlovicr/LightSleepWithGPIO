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

//definirajmo pin , ki ga bomo uporabili za prebujanje čipa
#define WAKEUP_PIN 12

void app_main(void)
{

    rtc_gpio_init(WAKEUP_PIN);
    rtc_gpio_set_direction(WAKEUP_PIN,RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pullup_en(WAKEUP_PIN); //nastavimo pull-up , nastavitev na vrednost HIGH
    rtc_gpio_pulldown_dis(WAKEUP_PIN);    //onemogočimo pull-down
    rtc_gpio_wakeup_enable(WAKEUP_PIN,GPIO_INTR_LOW_LEVEL);

    esp_sleep_enable_gpio_wakeup(); //omogočimo prebujanje preko GPIO pina
    esp_sleep_enable_timer_wakeup(5000000);

    while(true)
    {

        if(rtc_gpio_get_level(WAKEUP_PIN) == 0)
        {
            printf("sprosti gumb\n");
            do{
                vTaskDelay(pdMS_TO_TICKS(10));
            }while(rtc_gpio_get_level(WAKEUP_PIN) == 0);
        }
                
        printf("gremo na tirolsko\n"); 

        uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM); 

        int64_t prej = esp_timer_get_time();
        esp_light_sleep_start();
        int64_t sedaj = esp_timer_get_time();

        esp_sleep_wakeup_cause_t vzrok = esp_sleep_get_wakeup_cause();
        if(vzrok == ESP_SLEEP_WAKEUP_GPIO)
        {
            printf("prebujen s strani GPIO pina %d\n",WAKEUP_PIN);
        }
        else if(vzrok == ESP_SLEEP_WAKEUP_TIMER)
        {
            printf("prebujen s strani časovnika\n");
        }
        else
        {
            printf("prebujen iz neznanega razloga %d\n",vzrok);
        }   
        printf("prespal sem %lld milisekund\n", (sedaj - prej)/1000);
      
    }
}