#ifndef _intent_processor_h_
#define _intent_processor_h_

#include <map>
#include "WitAiChunkedUploader.h"

class Speaker;
class handleFB;

enum IntentResult
{
    FAILED,
    SUCCESS,
    SILENT_SUCCESS // success but don't play ok sound
};

class IntentProcessor
{
private:
    //std::map<std::string, int> m_device_to_pin;
    IntentResult turnOnDevice(const Intent &intent);
    IntentResult tellJoke();
    IntentResult life();
    handleFB *m_firebase;
    Speaker *m_speaker;

public:
    IntentProcessor(Speaker *speaker, handleFB *firebase);
    
    IntentResult processIntent(const Intent &intent);
};

#endif
