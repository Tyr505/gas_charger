#include <SPI.h>


#define AO_PIN 34
#define RO_val 0.41
const double m = -0.473;
const double b = 1.413;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 5000; 


void setup()
{
  Serial.begin(9600);
  
  
  analogSetAttenuation(ADC_11db);
  Serial.println("Esquentando sensor MQ2..");
  delay(20000);   // Aguardando tempo de aquecimento

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
    Serial.println(String(ppm)+" ppm");
    if (ppm < 1000) {
      Serial.println("Seguro!");
    } else if (ppm >= 1000 && ppm <= 2000) {
      Serial.println("Presença de Vazamento!");
    } else if (ppm > 2000) {
      Serial.println("Alto vazamento!");
    }

}
}