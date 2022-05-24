#include "Firebase.h"


FireBase::FireBase(){
    m_wifi_client = new WiFiClientSecure();
    /* m_wifi_client->setInsecure();
    json.add("name", 1);
    json.set("livingroom", 99);
    json.set("kitchen", 10);
    
    //json.set("data/arr/[2]", 100);
    if(m_wifi_client->connect("smarthomeapp-d2e21-default-rtdb.firebaseio.com",443)){
        Serial.println("OK");
        Serial.println("Send POST request");
        m_wifi_client->println("PATCH /Users/iSRxswqPiwcVicxtKfiqvDMEhKJ2/device.json HTTP/1.1");
        m_wifi_client->println("host: smarthomeapp-d2e21-default-rtdb.firebaseio.com");
        m_wifi_client->println("Content-Type: application/json");
        m_wifi_client->print("Content-Length: ");
        m_wifi_client->print(json.serializedBufferLength());
        m_wifi_client->print("\n\n");
        json.toString(*m_wifi_client);
        Serial.print("Read response...");

        if(json.readFrom(*m_wifi_client)){
            Serial.println();
            json.toString(Serial, true);
            Serial.println("\n\nComplete");
        }else
            Serial.println("1failed\n");
        
    }
    else
        Serial.println(" f2ailed\n");
    m_wifi_client->stop(); */

}

bool FireBase::connect(){
     m_wifi_client->setInsecure();
    /* json.add("name", 1);
    json.set("livingroom", 99);
    json.set("kitchen", 10); */
    
    //json.set("data/arr/[2]", 100);
    if(m_wifi_client->connect("smarthomeapp-d2e21-default-rtdb.firebaseio.com",443)){
        Serial.println("OK");
        Serial.println("Send POST request");
        m_wifi_client->println("PATCH /Users/iSRxswqPiwcVicxtKfiqvDMEhKJ2/device.json HTTP/1.1");
        m_wifi_client->println("host: smarthomeapp-d2e21-default-rtdb.firebaseio.com");
        m_wifi_client->println("Content-Type: application/json");
        m_wifi_client->print("Content-Length: ");
        m_wifi_client->print(json.serializedBufferLength());
        m_wifi_client->print("\n\n");
        return true;
    }
    else
        return false;
    
}


void FireBase::sendData(String path,uint8_t val){
    json.set(path, val);
    if(connect()){
        json.toString(*m_wifi_client);
        Serial.print("Read response...");

        if(json.readFrom(*m_wifi_client)){
            Serial.println();
            json.toString(Serial, true);
            Serial.println("\n\nComplete");
        }else
            Serial.println("1failed\n");
        m_wifi_client->stop();
    }
    json.clear();
    
}

FireBase::~FireBase(){
    delete m_wifi_client;
}