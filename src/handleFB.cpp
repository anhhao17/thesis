#include <Arduino.h>
#include "handleFB.h"



handleFB::handleFB(){
    config.database_url = URL;
    config.api_key = API_KEY;
    Firebase.reconnectWiFi(true);
    if(Firebase.signUp(&config,&auth,"","")){
        Serial.println("Oke");
    }else Serial.printf("%s\n", config.signer.signupError.message.c_str());
    Firebase.begin(&config,&auth);
    
    /* 
    if (Firebase.RTDB.setBool(&fbdo, "LivingRoom/alarm/status", false)){
		Serial.println("PASSED");
		Serial.println("PATH: " + fbdo.dataPath());
		Serial.println("TYPE: " + fbdo.dataType());
  	}
	else Serial.println("Failed"); 
    */
}

void handleFB::sendData(String path, bool val){
    if (Firebase.RTDB.setBool(&fbdo, path, val)){
		Serial.println("PASSED");
		Serial.println("PATH: " + fbdo.dataPath());
		Serial.println("TYPE: " + fbdo.dataType());
  	}else Serial.println("Failed"); 
}
