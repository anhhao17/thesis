#include <Arduino.h>
#include "IndicatorLight.h"
#include "config.h"
// This task does all the heavy lifting for our application
void indicatorLedTask(void *param)
{
    IndicatorLight *indicator_light = static_cast<IndicatorLight *>(param);
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
    while (true)
    {
        // wait for someone to trigger us
        uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
        if (ulNotificationValue > 0)
        {
            switch (indicator_light->getState())
            {
            case indicator::OK:
            {
                digitalWrite(GREEN_LED,LOW);
                vTaskDelay(1000);
                digitalWrite(GREEN_LED,HIGH);
                vTaskDelay(1000);
                digitalWrite(GREEN_LED,LOW);
                vTaskDelay(1000);
                digitalWrite(GREEN_LED,HIGH);
                
                break;
            }
            case indicator::FAIL:
            {
                digitalWrite(RED_LED,LOW);
                vTaskDelay(1000);
                digitalWrite(RED_LED,HIGH);
                vTaskDelay(1000);
                digitalWrite(RED_LED,LOW);
                vTaskDelay(1000);
                digitalWrite(RED_LED,HIGH);
                
                break;
            }
            case indicator::OFF:
            {
                digitalWrite(RED_LED,HIGH);
                digitalWrite(GREEN_LED,HIGH);
                digitalWrite(BLUE_LED,HIGH);
    
                break;
            }
            case indicator::ON:
            {
                digitalWrite(BLUE_LED, LOW);
                break;
            }
            case indicator::PULSING:
            {
                // do a nice pulsing effect
                float angle = 0;
                while (indicator_light->getState() == indicator::PULSING)
                {
                    digitalWrite(RED_LED, LOW); // turn the LED on 
                    vTaskDelay(100); // wait for a second
                    digitalWrite(RED_LED, HIGH); // turn the LED off by making the voltage LOW
                    vTaskDelay(100); // wait for a second
                    digitalWrite(GREEN_LED, LOW); // turn the LED on 
                    vTaskDelay(100); // wait for a second
                    digitalWrite(GREEN_LED, HIGH); // turn the LED off by making the voltage LOW
                    vTaskDelay(100); // wait for a second
                    digitalWrite(BLUE_LED, LOW); // turn the LED on
                    vTaskDelay(100); // wait for a second
                    digitalWrite(BLUE_LED, HIGH); // turn the LED off by making the voltage LOW
                    vTaskDelay(100);
                }
            }
            }
        }
    }
}

IndicatorLight::IndicatorLight()
{
    // use the build in LED as an indicator - we'll set it up as a pwm output so we can make it glow nicely
    pinMode(RED_LED,OUTPUT);
    pinMode(GREEN_LED,OUTPUT);
    pinMode(BLUE_LED,OUTPUT);
    digitalWrite(RED_LED,HIGH);
    digitalWrite(GREEN_LED,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    // start off with the light off
    m_state = indicator::OFF;
    // set up the task for controlling the light
    xTaskCreate(indicatorLedTask, "Indicator LED Task", 4096, this, 1, &m_taskHandle);
}

 void IndicatorLight::setState(indicator::IndicatorState state)
{
    m_state = state;
    xTaskNotify(m_taskHandle, 1, eSetBits);
}

indicator::IndicatorState IndicatorLight::getState()
{
    return m_state;
}