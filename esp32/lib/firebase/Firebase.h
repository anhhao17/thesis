#ifndef FIREBASE
#define FIREBASE


#include "WiFiClientSecure.h"
//#include <ArduinoJson.h>
#include <FirebaseJson.h>
#include <vector>

#define numberOfDevice 8
class FireBase {
private:
    WiFiClientSecure *m_wifi_client;
    FirebaseJson json;
public:

    FireBase();
    ~FireBase();
    bool connect(const char *state);
    void sendData(String path, bool val);
    std::vector<String> getData();
    //std::map<String,int>Device_to_id;
};


#endif