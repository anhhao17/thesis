#ifndef _handle_h_
#define _handle_h_

#include <FirebaseESP32.h>

//Firebase project

#define API_KEY "AIzaSyBP_iz79fpA7izAG4Kq1n7PUPM_zSHJCzQ"
#define URL "smarthomeapp-d2e21-default-rtdb.firebaseio.com"



class handleFB{
private:

    
public:
    FirebaseData fbdo;
    FirebaseData stream;
    FirebaseAuth auth;
    FirebaseConfig config;
    handleFB();
    ~handleFB();
    void sendData(String path, bool val);
};










#endif