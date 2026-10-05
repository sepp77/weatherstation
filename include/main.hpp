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
#include <Arduino_JSON.h>


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
extern Adafruit_SSD1306 display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2_clock, u8g2_temp, u8g2_maxmin, u8g2_wlan, u8g2_icon; // Create an instance of the U8G2 font library for the clock and date
extern DHT dht; // Create an instance of the DHT sensor

// openweathermap.org API variables
extern String serverPath; // API endpoint for weather data
extern String jsonBuffer;

// wlan login credentials
extern const char* ssid;
extern const char* password;

// ------------- enum ---------------------
// Enum for message types for debug messages
enum msgType{
    MSG_INFO,
    MSG_WARNING,
    MSG_ERROR
};




#endif // MAIN_HPP