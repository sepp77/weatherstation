#include "main.hpp"
#include "userFunctions.hpp"
#include "weatherstation.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + String(CITY_NAME) + "," + String(COUNTRY_CODE) + "&appid=" + String(WEATHER_API_KEY) + "&units=metric";
String jsonBuffer;
U8G2_FOR_ADAFRUIT_GFX u8g2_clock, u8g2_temp, u8g2_maxmin, u8g2_wlan, u8g2_icon;
const char *ssid = "DIGI-N7fE";
const char *password = "mPUbpMMcC4";

void drawWeatherIcon(const String &icon);

void setup()
{
  debugMsg("Entering setup", MSG_INFO);

  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Initialize button pins as input with pull-up resistors
  pinMode(BUT_1, INPUT_PULLUP);
  pinMode(BUT_2, INPUT_PULLUP);
  pinMode(BUT_3, INPUT_PULLUP);
  pinMode(BUT_4, INPUT_PULLUP);

  // initialize wlan connection
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  // time configure (time zone: UTC+3)
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Initialize the DHT sensor
  Wire.begin();
  dht.begin();

  // initialize display with the I2C addr 0x3C (for the 128x64)
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    debugMsg("SSD1306 allocation failed", MSG_ERROR);
    while (true)
    {
    };
  }
  else
  {
    debugMsg("SSD1306 allocation successful", MSG_INFO);
  }
  u8g2_clock.begin(display);
  // u8g2_clock.setFont(u8g2_font_pxplusibmvga9_tn); // define font for u8g2 (https://github.com/olikraus/u8g2/wiki/fntlist12)
  u8g2_clock.setFont(u8g2_font_crox3hb_tf);

  u8g2_temp.begin(display);
  u8g2_temp.setFont(u8g2_font_osr21_tf); // define font for u8g2 (https://github.com/olikraus/u8g2/wiki/fntlist12)

  u8g2_maxmin.begin(display);
  u8g2_maxmin.setFont(u8g2_font_6x13_t_cyrillic);

  u8g2_wlan.begin(display);
  // u8g2_wlan.setFont(u8g2_font_open_iconic_all_2x_t);
  u8g2_wlan.setFont(u8g2_font_siji_t_6x10);

  u8g2_icon.begin(display);
  // u8g2_icon.setFont(u8g2_font_osr21_tf);
  u8g2_icon.setFont(u8g2_font_open_iconic_weather_4x_t);

  // Clear the buffer
  display.clearDisplay();
}

int count = 64;

void loop()
{
  // debugMsg("Entering loop", MSG_INFO);

  display.clearDisplay(); // Clear the display buffer
  char buf[16];           // Buffer for formatted strings

  // show WLAN status
  long rssi = WiFi.RSSI();
  int quality = 2 * (rssi + 100);
  quality = constrain(quality, 0, 100);
  u8g2_wlan.setCursor(0, 13);
  if (quality > 75)
  {
    u8g2_wlan.print("\ue21f");
  }
  else if (quality > 50)
  {
    u8g2_wlan.print("\ue220");
  }
  else if (quality > 25)
  {
    u8g2_wlan.print("\ue221");
  }
  else
  {
    u8g2_wlan.print("\ue222");
  }

  // show time
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    snprintf(buf, sizeof(buf),
             "%02d:%02d",
             timeinfo.tm_hour,
             timeinfo.tm_min);
    u8g2_clock.setCursor(SCREEN_WIDTH - u8g2_clock.getUTF8Width(buf), 15);
  }
  else
  {
    debugMsg("Failed to obtain time", MSG_WARNING);
    snprintf(buf, sizeof(buf),
             "--:--");
    u8g2_clock.setCursor(SCREEN_WIDTH - u8g2_clock.getUTF8Width(buf), 15);
  }
  u8g2_clock.print(buf);

  jsonBuffer = httpGETRequest(serverPath.c_str());
  Serial.println(jsonBuffer);
  JSONVar myObject = JSON.parse(jsonBuffer);

  String icon = myObject["weather"][0]["icon"];

  //Serial.print("JSON object = ");
  //Serial.println(myObject);
  Serial.print("Temperature: ");
  Serial.println(myObject["main"]["temp"]);
  Serial.print("Pressure: ");
  Serial.println(myObject["main"]["pressure"]);
  Serial.print("Humidity: ");
  Serial.println(myObject["main"]["humidity"]);
  Serial.print("Wind Speed: ");
  Serial.println(myObject["wind"]["speed"]);
  Serial.println("Weather Icon: " + icon);

  u8g2_icon.setCursor(60, 55);
  //drawWeatherIcon(icon);

  u8g2_icon.write(count);

  String msg = "Current icon code: " + String(count);

  debugMsg(msg.c_str(), MSG_INFO);

  count < 71 ? count++ : count = 64;

  display.display(); // Show the display buffer on the screen

  delay(2000);
}

void drawWeatherIcon(const String &icon)
{
  debugMsg(icon.c_str(), MSG_INFO);
  if (icon == "01d" || icon == "01n")
    u8g2_icon.write(69); // Sonne
  else if (icon == "02d" || icon == "02n")
      u8g2_icon.write(65); // Sonne + Wolke
  else if (icon == "03d" || icon == "03n" ||
             icon == "04d" || icon == "04n")
      u8g2_icon.write(64); // Wolken
  else if (icon == "09d" || icon == "09n" ||
             icon == "10d" || icon == "10n" || icon == "11d" || icon == "11n" || icon == "13d" || icon == "13n")
      u8g2_icon.write(67); // Regen
}