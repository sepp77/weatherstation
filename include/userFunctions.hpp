#ifndef USER_FUNCTIONS_HPP
#define USER_FUNCTIONS_HPP

#include <U8g2_for_Adafruit_GFX.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "main.hpp"
#include "weatherstation.hpp"

// ------------- function prototypes ---------------------
void debugMsg(const char *msg, msgType type);

String httpGETRequest(const char* serverName);

const char* getWlanStatus(long rssi);

const char *getTimeString();

uint8_t drawWeatherIcon(const String &icon);

float getTemperature();


#endif // USER_FUNCTIONS_HPP
