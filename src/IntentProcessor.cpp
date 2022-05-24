#include <Arduino.h>
#include "IntentProcessor.h"
#include "Speaker.h"
//#include "handleFB.h"
IntentProcessor::IntentProcessor(Speaker *speaker) 
{
    m_speaker = speaker;
    //setupFB();
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
    m_fire_base = new FireBase();
    bool is_turn_on = intent.trait_value == "bat";
    
    /* Serial.printf("Total heap: %d\n", ESP.getHeapSize());
        Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
        
        Serial.printf("livingroom/%d",is_turn_on);
            //m_firebase->sendData("livingroom/1",is_turn_on); */
    Serial.printf("file: %s, func: %s.47",__FILE__,__func__);
    if (intent.device_name == "khách" || intent.device_name == "livingroom" )
        m_fire_base->sendData("livingroom",is_turn_on);
    if(intent.device_name == "ngủ" || intent.device_name == "bedroom")
        m_fire_base->sendData("bedroom",is_turn_on);
    if(intent.device_name == "tắm" || intent.device_name == "bathroom" )
        m_fire_base->sendData("bathroom",is_turn_on);

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
    /* Serial.printf("Intent is %s\n", intent.intent_name.c_str());
    Serial.printf("Intent is %s\n", intent.device_name.c_str());
    Serial.printf("Intent is %s\n", intent.device_num.c_str());
    Serial.printf("Intent is %s\n", intent.trait_value.c_str());
    bool name = intent.device_name == "khách";
    Serial.println(name); */
    if (intent.intent_name == "on_off") //can
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

IntentProcessor::~IntentProcessor(){
    delete m_fire_base;
    m_fire_base = NULL;
    uint32_t free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram after firebase cleanup %d\n", free_ram);

}
