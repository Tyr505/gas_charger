#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

#define SCREEN_WIDTH 128 // largura display OLED, em pixels
#define SCREEN_HEIGHT 64 // altura display OLED, em pixels

// Conexão do display SSD1306 usando I2C
#define OLED_RESET     -1 // pino de RESET
#define SCREEN_ADDRESS 0x3C 
#define AO_PIN 34
#define RO_val 0.41
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const double m = -0.473;
const double b = 1.413;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 5000; 

const char* ssid = "Desktop_F8A15897";
const char* password = "825403620P956092";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+5519974208181";
String apiKey = "7951414";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup()
{
  Serial.begin(115200);
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Loop infinito caso não encontre o display
  }
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
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
  double ppm = pow(10,ppm_log)*100;

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
      delay(10000);
      sendMessage("Atenção! \nFoi detectada uma presença de vazamento de gás em sua casa. \nVerifique sua válvula!");
    } else if (ppm > 2000) {
      display.println("Alto vazamento!");
      delay(10000);
      sendMessage("ATENÇÃO \nAlto vazamento de gás detectado em sua casa. \nVerifique sua válvula imediatamente!");
    }
    display.display();
}
}