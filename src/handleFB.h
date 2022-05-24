/* #pragma once

#ifndef FB
#define FB

#include <FirebaseESP32.h>

//Firebase project

#define API_KEY "AIzaSyBP_iz79fpA7izAG4Kq1n7PUPM_zSHJCzQ"
#define URL "smarthomeapp-d2e21-default-rtdb.firebaseio.com"

FirebaseData fbdo;
//FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;




void setupFB(){
    config.database_url = URL;
    config.api_key = API_KEY;
    Firebase.reconnectWiFi(true);
    if(Firebase.signUp(&config,&auth,"","")){
        Serial.println("Oke");
    }else Serial.printf("%s\n", config.signer.signupError.message.c_str());
    Firebase.begin(&config,&auth);
}
void sendData(String path, int val){
    Serial.printf("Total heap: %d\n", ESP.getHeapSize());
	Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    if (Firebase.RTDB.setInt(&fbdo, path, val)){
		Serial.println("PASSED");
		Serial.println("PATH: " + fbdo.dataPath());
		Serial.println("TYPE: " + fbdo.dataType());
  	}else Serial.println("Failed"); 
    uint32_t free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram after firebase cleanup %d\n", free_ram);
}






#endif */