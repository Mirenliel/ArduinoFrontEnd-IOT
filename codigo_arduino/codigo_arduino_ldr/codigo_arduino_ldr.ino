#include "Adafruit_Sensor.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

#define LDRPIN A0

DHT dht(DHTPIN, DHTTYPE);

int limite_baixo = 300; 
int limite_alto = 600;

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT); 
  pinMode(led_vermelho, OUTPUT);
}

void loop() {
  delay(2000);

  // DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // LDR
  int luz = analogRead(LDRPIN);

// Apaga todos os LEDs primeiro 
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, LOW); 
  digitalWrite(led_vermelho, LOW);

 

// Comparação da luminosidade 
if (luz < limite_baixo) { 
  digitalWrite(led_vermelho, HIGH); }
else if (luz < limite_alto) { 
  digitalWrite(led_amarelo, HIGH); }
else { digitalWrite(led_verde, HIGH); }



  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler o DHT22!");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" % | ");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C | ");

  Serial.print("Luminosidade: ");
  Serial.println(luz);
}
