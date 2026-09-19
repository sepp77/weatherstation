#ifndef MAIN_HPP
#define MAIN_HPP

// ------------- includes ---------------------

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Grove_Temperature_And_Humidity_Sensor.h>

#include "weatherstation.hpp"

// ------------- user defines ---------------------

#define DEBUG 1

// button defines
#define BUT_1 0
#define BUT_2 1
#define BUT_3 4
#define BUT_4 6

// DHT sensor defines
#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// display defines
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C   // Address of SSD1306 display


// ----------- global variables -------------------

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //
DHT dht(DHTPIN, DHTTYPE); // Create an instance of the DHT sensor

// ------------- enum ---------------------
// Enum for message types for debug messages
enum msgType{
    MSG_INFO,
    MSG_WARNING,
    MSG_ERROR
};


// ------------- function prototypes ---------------------
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

#endif // MAIN_HPP