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

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

  if(digitalRead(BUT_1) == LOW){
    debugMsg("Button 1 pressed", MSG_INFO);
    display.setCursor(rand() % 128,rand() % 64); // Random position for demonstration
    display.println("But 1");
  }

  if(digitalRead(BUT_2) == LOW){
    debugMsg("Button 2 pressed", MSG_INFO);
    display.setCursor(rand() % 128,rand() % 64); // Random position for demonstration
    display.println("But 2");
  }

  if(digitalRead(BUT_3) == LOW){
    debugMsg("Button 3 pressed", MSG_INFO);
    display.setCursor(rand() % 128,rand() % 64); // Random position for demonstration
    display.println("But 3");
  }

  if(digitalRead(BUT_4) == LOW){
    debugMsg("Button 4 pressed", MSG_INFO);
    display.setCursor(rand() % 128,rand() % 64); // Random position for demonstration
    display.println("But 4");
  }

  display.display(); // Show the display buffer on the screen

  delay(10); 
}

