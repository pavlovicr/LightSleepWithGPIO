#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
//#include "esp_log.h"
//#include "esp_timer.h" 
//#include "driver/uart.h" 
#include "driver/rtc_io.h"


#define STIKALO GPIO_NUM_15


RTC_DATA_ATTR int steviloBujenj = 0;

void app_main(void)
{

   rtc_gpio_deinit ( STIKALO ) ; //se lahko uporablja kot navaden GPIO preden pride do komande , ki ga spremeni v EXT0

   

   rtc_gpio_pullup_en(STIKALO);
   rtc_gpio_pulldown_dis(STIKALO);
   esp_sleep_enable_ext0_wakeup(STIKALO, 0); //1 = HIGH, 0 = LOW 

   printf("Grem na tirolsko, zbudil sem se %d krat \n", steviloBujenj ++);   
    
   esp_deep_sleep_start();
    
    
}


     