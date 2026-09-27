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
  //u8g2_clock.setFont(u8g2_font_pxplusibmvga9_tn); // define font for u8g2 (https://github.com/olikraus/u8g2/wiki/fntlist12)
  u8g2_clock.setFont(u8g2_font_crox3hb_tf );

  u8g2_temp.begin(display);
  u8g2_temp.setFont(u8g2_font_osr21_tf); // define font for u8g2 (https://github.com/olikraus/u8g2/wiki/fntlist12)

  u8g2_maxmin.begin(display);
  u8g2_maxmin.setFont(u8g2_font_6x13_t_cyrillic);

  u8g2_wlan.begin(display);
  //u8g2_wlan.setFont(u8g2_font_open_iconic_all_2x_t);
  u8g2_wlan.setFont(u8g2_font_siji_t_6x10);
  
  // Clear the buffer
  display.clearDisplay();
}

void loop()
{
  //debugMsg("Entering loop", MSG_INFO);

  display.clearDisplay(); // Clear the display buffer
  char buf[16]; // Buffer for formatted strings 


  // show WLAN status
  long rssi = WiFi.RSSI();
  int quality = 2 * (rssi + 100);
  quality = constrain(quality, 0, 100);
  u8g2_wlan.setCursor(0, 13);
  if(quality >75){
    u8g2_wlan.print("\ue21f");
  } else if(quality > 50){
    u8g2_wlan.print("\ue220");
  } else if(quality > 25){
    u8g2_wlan.print("\ue221");
  } else{
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



  display.display(); // Show the display buffer on the screen

  delay(500);
}
