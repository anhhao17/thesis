#include <Arduino.h>
#include <WiFi.h>
#include <driver/i2s.h>
#include <esp_task_wdt.h>
#include "I2SMicSampler.h"
#include "I2SOutput.h"
#include "config.h"
#include "Application.h"
#include "SPIFFS.h"
#include "IntentProcessor.h"
#include "Speaker.h"
#include "IndicatorLight.h"
#include "connectWiFi.h"
#include <algorithm>
//#include "handleFB.h"

//extern WebServer server(80);

// i2s config for reading from both channels of I2S
i2s_config_t i2sMemsConfigBothChannels = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s microphone pins set up in config.h
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,		//32  
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,	//25
    .data_out_num = I2S_PIN_NO_CHANGE,		
    .data_in_num = I2S_MIC_SERIAL_DATA};	//33

// i2s speaker pins
i2s_pin_config_t i2s_speaker_pins = {
    .bck_io_num = I2S_SPEAKER_SERIAL_CLOCK,
    .ws_io_num = I2S_SPEAKER_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_SPEAKER_SERIAL_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE};

// This task does all the heavy lifting for our application
void applicationTask(void *param)
{
  Application *application = static_cast<Application *>(param);

  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  while (true)
  {
    // wait for some audio samples to arrive
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue > 0)
    {
      application->run();
    }
  }
}


/**
 * @brief Code Flow Describtion
 * 1. We set up I2S mic and speaker for the input and output.
 * 2. Make sure we connect wifi, instead of input directly we set up connect via Webserver, Across the connectWiFi to more details.
 * 3. Setting up firebase get user ID (TO-DO) and setup own ID.
 * 4. APP FLOW {
 * 
 * }
 *
 * 
 */
void setup()
{
	Serial.begin(115200);
	delay(1000);
	Serial.println("Starting up");
	// start up wifi
	// launch WiFi
	if(!connectWifi()){
		setupAP();
	}
	
	while ((WiFi.status() != WL_CONNECTED))
  	{
    	Serial.print(".");
    	delay(100);
    	handleClient();
  	}  

	//handleFB *fireBase = new handleFB();
	

	Serial.printf("Total heap: %d\n", ESP.getHeapSize());
	Serial.printf("Free heap: %d\n", ESP.getFreeHeap());

	// startup SPIFFS for playing wav file 
	// dont make sure use Speaker
	SPIFFS.begin();
	// make sure we don't get killed for our long running tasks
	esp_task_wdt_init(10, false);

	// start up the I2S input (from either an I2S microphone or Analogue microphone via the ADC)

	I2SSampler *i2s_sampler = new I2SMicSampler(i2s_mic_pins, false);



	// start the i2s speaker output
	I2SOutput *i2s_output = new I2SOutput();
	i2s_output->start(I2S_NUM_1, i2s_speaker_pins);
	Speaker *speaker = new Speaker(i2s_output);

	// indicator light to show when we are listening 
	IndicatorLight *indicator_light = new IndicatorLight();

	// and the intent processor
	IntentProcessor *intent_processor = new IntentProcessor(speaker);


	// create our application
	Application *application = new Application(i2s_sampler, intent_processor, speaker, indicator_light);

	// set up the i2s sample writer task
	TaskHandle_t applicationTaskHandle;
	xTaskCreate(applicationTask, "Application Task", 8192, application, 1, &applicationTaskHandle);

	// start sampling from i2s device 
	i2s_sampler->start(I2S_NUM_0, i2sMemsConfigBothChannels, applicationTaskHandle);

}

void loop()
{
  vTaskDelay(1000);
}