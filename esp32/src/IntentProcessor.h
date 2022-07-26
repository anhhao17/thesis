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
    std::vector<String> device;
    IntentResult turnOnDevice(const Intent &intent, bool status);
    IntentResult tellJoke();
    IntentResult life();
    
    Speaker *m_speaker;
    FireBase *m_fire_base;

public:
    void deleteFirebase();
    IntentProcessor(Speaker *speaker);
    ~IntentProcessor();
    IntentResult processIntent(Intent &intent);
    
};

#endif
