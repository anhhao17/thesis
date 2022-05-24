#ifndef FIREBASE
#define FIREBASE


#include "WiFiClientSecure.h"
//#include <ArduinoJson.h>
#include <FirebaseJson.h>



class FireBase {
private:
    WiFiClientSecure *m_wifi_client;
    FirebaseJson json;
public:

    FireBase();
    ~FireBase();
    bool connect();
    void sendData(String path, uint8_t val);
};


#endif