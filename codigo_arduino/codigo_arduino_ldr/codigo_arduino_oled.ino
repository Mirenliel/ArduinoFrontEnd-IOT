#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_Sensor.h"
#include "DHT.h"

// ======================================================
// OLED 0.96" SPI 128x64
// ======================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Ajuste esses pinos conforme sua ligação
#define OLED_SCK   13   // SDC / CLK
#define OLED_MOSI  11   // SDA / MOSI
#define OLED_CS    10
#define OLED_DC     9
#define OLED_RESET  8

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &SPI,
  OLED_DC,
  OLED_RESET,
  OLED_CS
);

// ======================================================
// DHT22
// ======================================================

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ======================================================
// LDR e LEDs
// ======================================================

#define LDRPIN A0

#define led_vermelho 7
#define led_amarelo 6
#define led_verde 5

// ======================================================
// Variáveis
// ======================================================

int luz;

float h;
float t;

unsigned long tempoAnterior = 0;

bool mostrarTemperatura = true;

// Tempo de cada tela
const unsigned long TEMPO_TELA = 6000;

// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(9600);

  // -----------------------
  // DHT22
  // -----------------------

  dht.begin();

  // -----------------------
  // LEDs
  // -----------------------

  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

  // -----------------------
  // OLED
  // -----------------------

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {

    Serial.println("Falha ao iniciar OLED!");

    while (true);
  }


  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);

  display.setCursor(8, 20);

  display.println("Iniciando");

  display.display();

  delay(2000);

  display.clearDisplay();
  display.display();
}

// ======================================================
// LOOP
// ======================================================

void loop() {

  // ====================================================
  // Leitura dos sensores
  // ====================================================

  h = dht.readHumidity();

  t = dht.readTemperature();

  luz = analogRead(LDRPIN);

  // ====================================================
  // LEDs de luminosidade
  // ====================================================

  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);

  if (luz >= 200) {

    digitalWrite(led_vermelho, HIGH);

  }
  else if (luz >= 100) {

    digitalWrite(led_amarelo, HIGH);

  }
  else {

    digitalWrite(led_verde, HIGH);
  }

  // ====================================================
  // Verifica DHT22
  // ====================================================

  if (isnan(h) || isnan(t)) {

    Serial.println("Falha ao ler o DHT22!");

    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(10, 20);
    display.println("Erro no DHT22!");

    display.setCursor(10, 35);
    display.println("Verifique sensor");

    display.display();

    delay(2000);

    return;
  }

  // ====================================================
  // Troca de tela a cada 6 segundos
  // ====================================================

  if (millis() - tempoAnterior >= TEMPO_TELA) {

    tempoAnterior = millis();

    mostrarTemperatura = !mostrarTemperatura;
  }

  // ====================================================
  // OLED
  // ====================================================

  display.clearDisplay();

  // ----------------------------------------------------
  // Tela 1
  // Temperatura + Umidade
  // ----------------------------------------------------

  if (mostrarTemperatura) {

    display.setTextSize(1);

    display.setCursor(23, 2);
    display.println("AMBIENTE");

    display.drawLine(0, 13, 127, 13, SSD1306_WHITE);

    // Temperatura
    display.setCursor(0, 21);
    display.print("Temp:");

    display.setTextSize(2);

    display.setCursor(38, 17);
    display.print(t, 1);

    display.setTextSize(1);
    display.print(" C");

    // Umidade
    display.setTextSize(1);

    display.setCursor(0, 45);
    display.print("Umid:");

    display.setTextSize(2);

    display.setCursor(38, 40);
    display.print(h, 1);

    display.setTextSize(1);
    display.print("%");
  }

  // ----------------------------------------------------
  // Tela 2
  // Luminosidade
  // ----------------------------------------------------

  else {

    display.setTextSize(1);

    display.setCursor(22, 2);
    display.println("LUMINOSIDADE");

    display.drawLine(0, 13, 127, 13, SSD1306_WHITE);

    display.setTextSize(3);

    if (luz < 10) {

      display.setCursor(54, 24);

    }
    else if (luz < 100) {

      display.setCursor(45, 24);

    }
    else if (luz < 1000) {

      display.setCursor(36, 24);

    }
    else {

      display.setCursor(27, 24);
    }

    display.print(luz);

    // Mostra também uma classificação

    display.setTextSize(1);

    if (luz >= 200) {

      display.setCursor(42, 55);
      display.print("CLARO");

    }
    else if (luz >= 100) {

      display.setCursor(36, 55);
      display.print("MEDIO");

    }
    else {

      display.setCursor(42, 55);
      display.print("ESCURO");
    }
  }

  display.display();

  // ====================================================
  // Monitor Serial
  // ====================================================

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" % | ");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C | ");

  Serial.print("Luminosidade: ");
  Serial.println(luz);

  // ====================================================
  // JSON
  // ====================================================

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
