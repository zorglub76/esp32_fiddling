#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>

#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

#define PIN_CLK 14
#define PIN_MOSI 13
#define PIN_RESET 25
#define PIN_DC 26
#define PIN_CS 27

#define MAX_APs 20

static char tag[] = "test_SSD1306";

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void) {
    nvs_flash_init();
    // initialize the tcp stack
	tcpip_adapter_init();

	// initialize the wifi event handler
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	// configure, initialize and start the wifi driver
	wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_start());

	// configure and run the scan process in blocking mode
	wifi_scan_config_t scan_config = {
		.ssid = 0,
		.bssid = 0,
		.channel = 0,
		.show_hidden = 0
	};

	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.clk   = PIN_CLK;
	u8g2_esp32_hal.mosi  = PIN_MOSI;
	u8g2_esp32_hal.cs    = PIN_CS;
	u8g2_esp32_hal.dc    = PIN_DC;
	u8g2_esp32_hal.reset = PIN_RESET;
	u8g2_esp32_hal_init(u8g2_esp32_hal);


	u8g2_t u8g2; // a structure which will contain all the data for one display
	u8g2_Setup_ssd1306_128x64_noname_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_msg_comms_cb,
		u8g2_esp32_msg_gpio_and_delay_cb);  // init u8g2 structure

	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_helvR08_tf);
    u8g2_DrawStr(&u8g2, 0,15,"Network scanner");
	u8g2_SendBuffer(&u8g2);
	vTaskDelay(2000 / portTICK_PERIOD_MS);

	char asdf[3];

	while(1){
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 1));
		// get the list of APs found in the last scan
		uint16_t ap_num = MAX_APs;
		wifi_ap_record_t ap_records[MAX_APs];
		ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));

		u8g2_ClearBuffer(&u8g2);

	    for(int i = 0; i < ap_num; i++){
		    u8g2_SetFont(&u8g2, u8g2_font_helvR08_tf);
	    	sprintf(asdf, "%3ddB", ap_records[i].rssi);
	    	u8g2_DrawStr(&u8g2, 0,12*(i+1), (char *)asdf);
		    u8g2_SetFont(&u8g2, u8g2_font_helvB08_tf);
	    	u8g2_DrawStr(&u8g2, 32,12*(i+1), (char *)ap_records[i].ssid);
	    }
		u8g2_SendBuffer(&u8g2);

		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

	ESP_LOGD(tag, "All done!");

	vTaskDelete(NULL);
}
