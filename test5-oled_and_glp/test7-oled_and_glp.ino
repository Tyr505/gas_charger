#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // largura display OLED, em pixels
#define SCREEN_HEIGHT 64 // altura display OLED, em pixels

// Conexão do display SSD1306 usando I2C
#define OLED_RESET     -1 // pino de RESET
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define AO_PIN 34
#define RO_val 0.41
const double m = -0.473;
const double b = 1.413;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 2000; 

void setup()
{
  Serial.begin(9600);
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Loop infinito caso não encontre o display
  }
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  analogSetAttenuation(ADC_11db);
  display.println("Esquentando sensor MQ2..");
  display.display();
  delay(20000);   // Aguardando tempo de aquecimento
  display.clearDisplay();
  display.setTextSize(2);
}

void loop() {

  double voltage = (analogRead(AO_PIN))*(5)/1023;
  double rs = (5.0 - voltage)/voltage;
  double rs_ro_ratio = rs/RO_val;
  double ppm_log = log((rs_ro_ratio-b)/m);
  double ppm = pow(10,ppm_log);

   currentMillis = millis(); 
if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Atualiza o tempo anterior
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(String(ppm)+" ppm");
    if (ppm < 1000) {
      display.println("Seguro!");
    } else if (ppm >= 1000 && ppm <= 2000) {
      display.println("Presença de Vazamento!");
    } else if (ppm > 2000) {
      display.println("Alto vazamento!");
    }
    display.display();
}
}
