#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "Adafruit_Sensor.h"
#include "DHT.h"

// =======================
// LCD
// =======================
#define LCD_ENDERECO 0x27
#define LCD_COLUNAS 16
#define LCD_LINHAS 2

LiquidCrystal_I2C lcd(LCD_ENDERECO, LCD_COLUNAS, LCD_LINHAS);

// =======================
// DHT22
// =======================
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// =======================
// LDR e LEDs
// =======================
#define LDRPIN A0

#define led_vermelho 7
#define led_amarelo 6
#define led_verde 5

// =======================
// Variáveis
// =======================
int luz;

float h;
float t;

unsigned long tempoAnterior = 0;
bool mostrarTemperatura = true;

// =======================
// SETUP
// =======================
void setup() {

  Serial.begin(9600);

  // DHT
  dht.begin();

  // LEDs
  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");

  delay(2000);
}

// =======================
// LOOP
// =======================
void loop() {

  // Lê os sensores
  h = dht.readHumidity();
  t = dht.readTemperature();

  luz = analogRead(LDRPIN);

  // =======================
  // LEDs de luminosidade
  // =======================

  // Apaga todos primeiro
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);

  if (luz > 200) {

    digitalWrite(led_vermelho, HIGH);

  }
  else if (luz > 100 && luz < 200) {

    digitalWrite(led_amarelo, HIGH);

  }
  else if (luz < 100) {

    digitalWrite(led_verde, HIGH);

  }

  // =======================
  // Verifica DHT22
  // =======================

  if (isnan(h) || isnan(t)) {

    Serial.println("Falha ao ler o DHT22!");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro no DHT22!");

    delay(2000);

    return;
  }

  // =======================
  // LCD
  // =======================

  // Troca a tela a cada 2 segundos
  if (millis() - tempoAnterior >= 2000) {

    tempoAnterior = millis();

    mostrarTemperatura = !mostrarTemperatura;

    lcd.clear();
  }

  if (mostrarTemperatura) {

    // Temperatura
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t, 1);
    lcd.print((char)223);
    lcd.print("C");

    // Umidade
    lcd.setCursor(0, 1);
    lcd.print("Umid: ");
    lcd.print(h, 1);
    lcd.print("%");

  } else {

    // Luminosidade
    lcd.setCursor(0, 0);
    lcd.print("Luminosidade:");

    lcd.setCursor(0, 1);
    lcd.print("Luz: ");
    lcd.print(luz);

  }

  // =======================
  // Monitor Serial
  // =======================

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" % | ");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C | ");

  Serial.print("Luminosidade: ");
  Serial.println(luz);

  // =======================
  // JSON
  // =======================

  Serial.print("{");

  Serial.print("\"temperatura\":");
  Serial.print(t);

  Serial.print(",\"umidade\":");
  Serial.print(h);

  Serial.print(",\"luminosidade\":");
  Serial.print(luz);

  Serial.println("}");

  delay(500);
}
