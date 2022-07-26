#include <Arduino.h>
#include "WiFi.h"
#include "EEPROM.h" 


#include "WebServer.h"

WebServer server(80);
String content;
String st;
int statusCode;

void createWebServer(void);
void setupAP(void);
void launchWeb(void); 
bool connectWifi();
void handleClient();

void handleClient(){

	server.handleClient();

}
bool connectWifi(){
  
  char ssid[32];
  char pass[32];

	WiFi.disconnect();
    EEPROM.begin(64);

    if(EEPROM.read(0)!=0){
        strcpy(ssid,EEPROM.readString(0).c_str());
        strcpy(pass,EEPROM.readString(32).c_str());
    }
    else{
        return false;
    }

    Serial.printf("SSID %s\n",ssid);
    Serial.printf("PASS: %s\n",pass);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,pass);
    if(WiFi.waitForConnectResult()!=WL_CONNECTED){
        return false;
        Serial.println("FAILED");
        
    }
    return true;
}
void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Truy cap IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}


void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("THESIS", "");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}




void createWebServer()
{
 {
    server.on("/", []() {

      IPAddress ip = WiFi.softAPIP();
      //IPAddress ip(192,168,5,1);
      //IPAddress nMask(255,255,255,0);
      //WiFi.softAPConfig(ip,ip,nMask);
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Hello from K18 FETEL at ";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='post'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });

    server.on("/post", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      String userId = server.arg("userid");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 64; ++i) 
          EEPROM.write(i, 0);
        
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i,qsid[i]); 
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32+i,qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.restart();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  } 
}

