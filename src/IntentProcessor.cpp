#include <Arduino.h>
#include "IntentProcessor.h"
#include "Speaker.h"
#include "handleFB.h"
IntentProcessor::IntentProcessor(Speaker *speaker, handleFB *firebase) 
{
    m_speaker = speaker;
    m_firebase = firebase;
}

//Send data to firebase
IntentResult IntentProcessor::turnOnDevice(const Intent &intent)
{
    if (intent.intent_confidence < 0.8)
    {
        Serial.printf("Only %.f%% certain on intent\n", 100 * intent.intent_confidence);
        return FAILED;
    }
    if (intent.device_name.empty())
    {
        Serial.println("No device found");
        return FAILED;
    }
    if (intent.device_confidence < 0.8)
    {
        Serial.printf("Only %.f%% certain on device\n", 100 * intent.device_confidence);
        return FAILED;
    }
    if (intent.trait_value.empty())
    {
        Serial.println("Can't work out the intent action");
        return FAILED;
    }
    if (intent.trait_confidence < 0.8)
    {
        Serial.printf("Only %.f%% certain on trait\n", 100 * intent.trait_confidence);
        return FAILED;
    }
    bool is_turn_on = intent.trait_value == "on";

    // turn on all lights
    if (intent.device_name == "lights")
    {
        
    }
    else
    {

        // see if the device name is something we know about
       /*  if (m_device_to_pin.find(intent.device_name) == m_device_to_pin.end())
        {
            Serial.printf("Don't recognise the device '%s'\n", intent.device_name.c_str());
            return FAILED;
        }
        digitalWrite(m_device_to_pin[intent.device_name], is_turn_on); */
    }
    // success!
    return SUCCESS;
}
 
IntentResult IntentProcessor::tellJoke()
{
    m_speaker->playRandomJoke();
    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::life()
{
    m_speaker->playLife();
    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::processIntent(const Intent &intent)
{
    if (intent.text.empty())
    {
        Serial.println("No text recognised");
        return FAILED;
    }
    Serial.printf("I heard \"%s\"\n", intent.text.c_str());
    if (intent.intent_name.empty())
    {
        Serial.println("Can't work out what you want to do with the device...");
        return FAILED;
    }
    Serial.printf("Intent is %s\n", intent.intent_name.c_str());
    if (intent.intent_name == "Turn_on_device") //can
    {
        //send to firebase
        return turnOnDevice(intent);
    }
    if (intent.intent_name == "Tell_joke")
    {
        return tellJoke();
    }
    if (intent.intent_name == "Life")
    {
        return life();
    }

    return FAILED;
}

