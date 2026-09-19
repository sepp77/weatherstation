#include "main.hpp"



void setup() {
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  // time configure (time zone: UTC+3)
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Initialize the DHT sensor
  Wire.begin();
  dht.begin();

  // initialize display with the I2C addr 0x3C (for the 128x64)
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    debugMsg("SSD1306 allocation failed", MSG_ERROR);
    while(true){};
  }
  else{
    debugMsg("SSD1306 allocation successful", MSG_INFO);
  }
  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  debugMsg("Entering loop", MSG_INFO);

  display.clearDisplay(); // Clear the display buffer

  // Read temperature and humidity from the DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity)) {
    debugMsg("Failed to read from DHT sensor!", MSG_ERROR);
    return;
  }

  // Display temperature and humidity on the OLED display
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner

  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  // show time
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    display.setCursor(0, 32); // Move cursor to the next line
    display.printf("Time: %02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  } else {
    debugMsg("Failed to obtain time", MSG_WARNING);
  }

  display.display(); // Show the display buffer on the screen

  delay(10); 
}

