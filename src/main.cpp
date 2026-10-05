#include "main.hpp"
#include "userFunctions.hpp"
#include "weatherstation.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
String serverPath = "http://api.openweathermap.org/data/2.5/forecast?q=" + String(CITY_NAME) + "," + String(COUNTRY_CODE) + "&appid=" + String(WEATHER_API_KEY) + "&units=metric";
String jsonBuffer;
U8G2_FOR_ADAFRUIT_GFX u8g2_clock, u8g2_temp, u8g2_maxmin, u8g2_wlan, u8g2_icon;
const char *ssid = "DIGI-N7fE";
const char *password = "mPUbpMMcC4";

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
  u8g2_maxmin.setFont(u8g2_font_6x13_tf);

  u8g2_wlan.begin(display);
  // u8g2_wlan.setFont(u8g2_font_open_iconic_all_2x_t);
  u8g2_wlan.setFont(u8g2_font_siji_t_6x10);

  u8g2_icon.begin(display);
  // u8g2_icon.setFont(u8g2_font_osr21_tf);
  u8g2_icon.setFont(u8g2_font_open_iconic_weather_4x_t);

  // Clear the buffer
  display.clearDisplay();
}

void loop()
{
  debugMsg("Entering loop", MSG_INFO);

  display.clearDisplay(); // Clear the display buffer

  // show WLAN status
  u8g2_wlan.setCursor(0, 13);
  u8g2_wlan.print(getWlanStatus(WiFi.RSSI()));

  // show time
  const char *timeStr = getTimeString();
  u8g2_clock.setCursor(SCREEN_WIDTH - u8g2_clock.getUTF8Width(timeStr), 15);
  u8g2_clock.print(timeStr);

  // get weather data from openweathermap.org
  jsonBuffer = httpGETRequest(serverPath.c_str());
  JSONVar myObject = JSON.parse(jsonBuffer);
  String icon = myObject["list"][0]["weather"][0]["icon"];
  int tempOut = myObject["list"][0]["main"]["temp"];
  int temps[5];
  for (int i = 0; i < 5; i++)
  {
    temps[i] = myObject["list"][i]["main"]["temp_max"];
  }
  int *tempRange = getTempRange(temps, 5);

  Serial.println("Weather data: " + jsonBuffer);

  // show weather icon
  u8g2_icon.setCursor(SCREEN_WIDTH - 16 - 27, 53);
  u8g2_icon.write(drawWeatherIcon(icon));

  // show temperature
  // u8g2_temp.setCursor(0, SCREEN_HEIGHT - 10);
  static char buf[16];

  if (millis() % 10000 < 5000)
  {
    snprintf(buf, sizeof(buf), "%d°C", tempOut);
    u8g2_temp.setCursor(SCREEN_WIDTH - 55 - u8g2_temp.getUTF8Width(buf), SCREEN_HEIGHT - 12);
    debugMsg("outdoor Temp.", MSG_INFO);
  }
  else
  {
    snprintf(buf, sizeof(buf), "%.0f°C", getTemperature());
    u8g2_temp.setCursor(SCREEN_WIDTH - 55 - u8g2_temp.getUTF8Width(buf), SCREEN_HEIGHT - 12);
    debugMsg("sensor Temp.", MSG_INFO);
  }
  u8g2_temp.print(buf);
  debugMsg(buf, MSG_INFO);

  // show max temperature
  snprintf(buf, sizeof(buf), "%d° / %d°", tempRange[0], tempRange[1]);
  u8g2_maxmin.setCursor(SCREEN_WIDTH - u8g2_maxmin.getUTF8Width(buf) - 3, SCREEN_HEIGHT - 1);
  u8g2_maxmin.print(buf);

  display.display(); // Show the display buffer on the screen

  delay(1000); // Wait for 1 second before the next update
}
