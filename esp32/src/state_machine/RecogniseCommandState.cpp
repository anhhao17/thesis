#include <Arduino.h>
#include <ArduinoJson.h>
#include "I2SSampler.h"
#include "RingBuffer.h"
#include "RecogniseCommandState.h"
#include "IndicatorLight.h"
#include "Speaker.h"
#include "IntentProcessor.h"
#include "WitAiChunkedUploader.h"
#include "../config.h"
#include <string.h>

#define WINDOW_SIZE 320
#define STEP_SIZE 160
#define POOLING_SIZE 6
#define AUDIO_LENGTH 16000

RecogniseCommandState::RecogniseCommandState(I2SSampler *sample_provider, IndicatorLight *indicator_light, Speaker *speaker, IntentProcessor *intent_processor)
{
    // save the sample provider for use later
    m_sample_provider = sample_provider;
    m_indicator_light = indicator_light;
    m_speaker = speaker;
    m_intent_processor = intent_processor;
    m_speech_recogniser = NULL;
}
void RecogniseCommandState::enterState()
{
    // indicate that we are now recording audio
    m_indicator_light->setState(indicator::ON);
    m_speaker->playReady();

    /*3s waiting start*/
    m_start_time = millis();
    m_elapsed_time = 0;
    m_last_audio_position = -1;

    m_speech_recogniser = new WitAiChunkedUploader(COMMAND_RECOGNITION_ACCESS_KEY);

    Serial.println("Enter Recognise State");
}
bool RecogniseCommandState::run()
{
    // Connecting to WitAi 
    if (!m_speech_recogniser || !m_speech_recogniser->connected())
    {
        Serial.println("Error - Attempt to run with no http client");
        return true;
    }
    if (m_last_audio_position == -1)
    {
        
        m_last_audio_position = m_sample_provider->getCurrentWritePosition() - 16000;
    }
    // how many samples have been captured since we last ran
    int audio_position = m_sample_provider->getCurrentWritePosition();
    // work out how many samples there are taking into account that we can wrap around
    int sample_count = (audio_position - m_last_audio_position + m_sample_provider->getRingBufferSize()) % m_sample_provider->getRingBufferSize();
    // Serial.printf("Last sample position %d, current position %d, number samples %d\n", m_last_audio_position, audio_position, sample_count);
    if (sample_count > 0)
    {
        
        // send the samples to the server
        m_speech_recogniser->startChunk(sample_count * sizeof(int16_t));
        RingBufferAccessor *reader = m_sample_provider->getRingBufferReader();
        reader->setIndex(m_last_audio_position);
        // send the samples up in chunks
        int16_t sample_buffer[500];
        while (sample_count > 0)
        {
            for (int i = 0; i < sample_count && i < 500; i++)
            {
                sample_buffer[i] = reader->getCurrentSample();
                reader->moveToNextSample();
            }
            m_speech_recogniser->sendChunkData((const uint8_t *)sample_buffer, std::min(sample_count, 500) * 2);
            sample_count -= 500;
        }
        m_last_audio_position = reader->getIndex();
        m_speech_recogniser->finishChunk();
        delete (reader);

        // check has 3 seconds passed?
        unsigned long current_time = millis();
        m_elapsed_time += current_time - m_start_time;
        m_start_time = current_time;
        if (m_elapsed_time > 3000)
        {
            /*Blink led waiting indication*/
            m_indicator_light->setState(indicator::PULSING);

           
            
            Intent intent = m_speech_recogniser->getResults();
            delete m_speech_recogniser;
            m_speech_recogniser = NULL;
            IntentResult intentResult = m_intent_processor->processIntent(intent);
            m_indicator_light->setState(indicator::OFF);
            switch (intentResult)
            {
            
            case SUCCESS:
                Serial.println("Success");
                m_speaker->playOK();
                m_indicator_light->setState(indicator::OK);
                break;
            case FAILED:
                Serial.println("Failed");
                m_speaker->playCantDo();
                m_indicator_light->setState(indicator::FAIL);
                break;
            case SILENT_SUCCESS:
                // nothing to do
                break;
            }
            // indicate that we are done
            delay(500);
            m_indicator_light->setState(indicator::OFF);
            m_intent_processor->deleteFirebase();
            return true;
        }
    }
    /*Return false not done during 3s still in recognize state*/
    return false;
}

void RecogniseCommandState::exitState()
{
    
    //delete m_speech_recogniser 

}