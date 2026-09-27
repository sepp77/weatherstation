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
