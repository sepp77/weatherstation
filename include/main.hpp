#ifndef MAIN_HPP
#define MAIN_HPP

// ------------- includes ---------------------

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Grove_Temperature_And_Humidity_Sensor.h>
#include <WiFi.h>
#include <time.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <HTTPClient.h>

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

// openweathermap.org API key
#define WEATHER_API_KEY "dab44477f74001d541ec8332ceef27fa" // Replace with
#define CITY_NAME "Danes"
#define COUNTRY_CODE "RO"

// ----------- global variables -------------------

// peripheral objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //
U8G2_FOR_ADAFRUIT_GFX u8g2_clock, u8g2_temp, u8g2_maxmin, u8g2_wlan; // Create an instance of the U8G2 font library for the clock and date
DHT dht(DHTPIN, DHTTYPE); // Create an instance of the DHT sensor

// openweathermap.org API variables
String weatherApiUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + String(CITY_NAME) + "," + String(COUNTRY_CODE) + "&appid=" + String(WEATHER_API_KEY) + "&units=metric";

// wlan login credentials
const char* ssid     = "DIGI-N7fE";
const char* password = "mPUbpMMcC4";

// ------------- enum ---------------------
// Enum for message types for debug messages
enum msgType{
    MSG_INFO,
    MSG_WARNING,
    MSG_ERROR
};




#endif // MAIN_HPP