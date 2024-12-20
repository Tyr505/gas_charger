#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(9600);
  
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  pinMode(DO_PIN, INPUT);
  display.println("Warming up the MQ2 sensor");
  display.display();
  delay(20000);  // wait for the MQ2 to warm up
  // Clear the buffer.
  display.clearDisplay();

}

void loop() {
  int gasState = digitalRead(DO_PIN);

  if (gasState == HIGH)
    display.println("The gas is NOT present");
    display.display();
  else
    display.println("The gas is present");
    display.display();

}