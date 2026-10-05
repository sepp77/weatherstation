#include "userFunctions.hpp"
#include "main.hpp"

void debugMsg(const char *msg, msgType type){
    if (DEBUG){
        switch (type){
            case MSG_INFO:
                Serial.print("[INFO]\t");
                break;
            case MSG_WARNING:
                Serial.print("[WARNING]\t");
                break;
            case MSG_ERROR:
                Serial.print("[ERROR]\t");
                break;
        }
      Serial.println(msg);
    }
};

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

