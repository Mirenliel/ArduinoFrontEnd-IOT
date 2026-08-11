#include "Adafruit_Sensor.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

#define LDRPIN A0

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht.begin();
}

void loop() {
  delay(2000);

  // DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // LDR
  int luz = analogRead(LDRPIN);

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