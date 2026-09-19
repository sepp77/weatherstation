#include "main.hpp"

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
  u8g2_clock.setFont(u8g2_font_pxplusibmvga9_tn); // define font for u8g2 (https://github.com/olikraus/u8g2/wiki/fntlist12)

  u8g2_temp.begin(display);
  u8g2_temp.setFont(u8g2_font_osr21_tf); // define font for u8g2 (https://github.com/olikraus/u8g2/wiki/fntlist12)

  u8g2_wlan.begin(display);
  u8g2_wlan.setFont(u8g2_font_open_iconic_embedded_2x_t);
  
  // Clear the buffer
  display.clearDisplay();
}

void loop()
{
  debugMsg("Entering loop", MSG_INFO);

  display.clearDisplay(); // Clear the display buffer

  // Read temperature and humidity from the DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity))
  {
    debugMsg("Failed to read from DHT sensor!", MSG_ERROR);
    return;
  }

  // Display temperature and humidity on the OLED display
  u8g2_temp.setCursor(0, SCREEN_HEIGHT - 5); // Cursor unten links der Baseline

  if(millis() % 10000 < 5000){ // switch between temp and humidity every 2.5 second
    u8g2_temp.print(temperature);
    u8g2_temp.print("\u00B0C");
  } else{
    u8g2_temp.print(humidity);
    u8g2_temp.print(" %");
  }

  // show time
  struct tm timeinfo;

  u8g2_clock.setCursor(0, 10); // Cursor oben links der Baseline
  u8g2_clock.print("\u0042");

  if (getLocalTime(&timeinfo))
  {
    char buffer[32];
    snprintf(buffer, sizeof(buffer),
             "%02d:%02d",
             timeinfo.tm_hour,
             timeinfo.tm_min);
    u8g2_clock.setCursor(SCREEN_WIDTH - u8g2_clock.getUTF8Width(buffer), 10);
    u8g2_clock.print(buffer);
  }
  else
  {
    debugMsg("Failed to obtain time", MSG_WARNING);
  }

  display.display(); // Show the display buffer on the screen

  delay(100);
}
