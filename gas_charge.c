/*************************************************************
BLYNK WI-FI CONNECTION
  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP32 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL22KB59kWj"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "QbFnbnuP4N46qRzD-VRcV-5PR-3DJXEc"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include "soc/rtc.h"
#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2)

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

BlynkTimer timer;

const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

long empty_gallon = 0;

HX711 scale;
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  lcd.init();
  lcd.backlight();
}

void loop()
{
  Blynk.run();
  timer.run();
  lcd.clear();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  if (scale.is_ready()) {
    scale.set_scale();    
    lcd.print("Avaliando...Remova o botijão");
    delay(5000);
    scale.tare();
    lcd.clear()
    lcd.print("Completo!");
    lcd.setCursor(0, 1);
    lcd.print("Coloque o botijão...");
    delay(5000);
    long reading_ref = scale.get_units(10);
    lcd.clear
    lcd.print("Peso atual: ");
    lcd.setCursor(6, 1);
    lcd.print(reading_ref)
    lcd.setCursor(8, 1);
    lcd.print("kg"); 
    delay(5000);
    lcd.clear()
    lcd.print("Monitorando gás!");
    delay(5000);
    lcd.clear()
   //Galão cheio
    for(i=1; i<=16;i++){
      lcd.setCursor(i,1);
      lcd.print(255);
      lcd.setCursor(i,2);
      lcd.print(255);
    }
    delay(5000);
    weights_list [16] = {empty_gallon, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    //Dividindo o peso do galão em 16 intervalos do display
    for (i=1; i<=16; i++){
        reading_ref_div = (reading_ref-empty_gallon)/16; 
        weights_list[i] = weights_list[i-1] + reading_ref_div;     
    }
    
    i = 16;
    //Esvaziando galão
    while(i>=0 && i<=16){
      reading_actual = scale.get_units(10);
      if(reading_actual == weights_list[i]){
        lcd.setCursor(i,1);
        lcd.print(0);
        lcd.setCursor(i,2);
        lcd.print(0);
        i--;
      }
      
    }
    
  } 
  else {
    Serial.println("Erro, conversão de peso não encontrada");
  }
  delay(1000);
}
