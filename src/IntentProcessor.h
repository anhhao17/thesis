#ifndef _intent_processor_h_
#define _intent_processor_h_

#include <map>
#include "WitAiChunkedUploader.h"
#include "Firebase.h"
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
    
    Speaker *m_speaker;
    FireBase *m_fire_base;

public:
    IntentProcessor(Speaker *speaker);
    ~IntentProcessor();
    IntentResult processIntent(const Intent &intent);
};

#endif
