#include <Arduino.h>
#include "IntentProcessor.h"
#include "Speaker.h"
#include <algorithm>
//#include "handleFB.h"
IntentProcessor::IntentProcessor(Speaker *speaker) 
{
    m_speaker = speaker;
   
    //setupFB();
}

//Send data to firebase
IntentResult IntentProcessor::turnOnDevice(const Intent &intent,bool status)
{
    if (intent.intent_confidence < 0.8)
    {
        Serial.printf("INFO: file: %s:%d, msg: Only %.f%% certain on intent\n",__FILE__,__LINE__,100 * intent.intent_confidence);
        return FAILED;
    }

    for(int i = 0;i < numberOfDevice; i++){
        if(intent.text.find(device[i].c_str())!=std::string::npos){
           Serial.printf("INFO: file: %s:%d, msg: Find Device\n",__FILE__,__LINE__);
           char buff[10];
           sprintf(buff, "%d/status", i);
           m_fire_base->sendData(buff,status);
           return SUCCESS;
        }//else Serial.printf("INFO: file: %s:%d, msg: Cannot matches device id %d\n",__FILE__,__LINE__,i);

    }
    
   
    
    return FAILED;
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

IntentResult IntentProcessor::processIntent(Intent &intent)
{
    std::transform(intent.text.begin(),intent.text.end(),intent.text.begin(),::tolower);
    //std::transform(intent.text.begin(),intent.text.end(),intent.text.begin(),::tolower);
    m_fire_base = new FireBase();
    if (intent.text.empty())
    {
        Serial.printf("INFO: file: %s:%d, msg: No text recognise\n",__FILE__,__LINE__);
        //Serial.println("No text recognised");
        return FAILED;
    }
    Serial.printf("INFO: file: %s:%d, msg: Heard \"%s\"\n",__FILE__,__LINE__, intent.text.c_str());
    
    if (intent.text.find("cập nhật")!=std::string::npos) //can
    {
        //update firebase
        device = m_fire_base->getData();
        return SUCCESS;
    }
    if(device.size()!=0){
        /* if (!strncmp(intent.text.c_str(),"bật",3)){
            return turnOnDevice(intent,true);
        }
        if(!strncmp(intent.text.c_str(),"tắt",3)){
            return turnOnDevice(intent,false);
        } */
        if(intent.text.find("bật")!=std::string::npos)
            return turnOnDevice(intent,true);
        if(intent.text.find("tắt")!=std::string::npos)
            return turnOnDevice(intent,false);
        if(intent.text.find("ke chuyen")!=std::string::npos)
            return tellJoke();
    }

    return FAILED;
    
}
void IntentProcessor::deleteFirebase(){
    delete m_fire_base;
    m_fire_base = NULL; 
}
IntentProcessor::~IntentProcessor(){
    delete m_speaker;
    m_speaker = NULL;
    

}
