#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <DHT.h>

/* Read RFID Tag with RC522 RFID Reader
 *  Made by miliohm.com
 */

#include <MFRC522.h>
#include <SPI.h>

// Modulo wifi
const char *ssid = "IFCE_Open"; // SSID Wifi
const char *password = "";      // Senha Wifi
ESP8266WebServer server(80);

constexpr uint8_t RST_PIN = D3; // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;
//#define RST_PIN 0 // 0
//#define SS_PIN 2 // 2
// Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

// variaveis das tags
int total = 2;
int externoQtd = 0;
int internoQtd = 2;
bool tag1Status = true;
bool tag2Status = true;
// tag1Id = '1355610852';
// tag2Id = '49919329';
char *dadosOvelha = "";

float TemperaturaLida = 0.0;
float UmidadeLida = 0.0;

// GPIO do NodeMCU que o pino de comunicação do sensor está ligado.
#define DHTPIN D1
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);
  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  // Módulo de tempertura e umidade
  dht.begin();
  TemperaturaLida = dht.readTemperature();
  UmidadeLida = dht.readHumidity();
  Serial.println("Temperatura");
  Serial.println(TemperaturaLida);
  Serial.println("Umidade");
  Serial.println(UmidadeLida);

  // Modulo wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Aguarda Conexão e Informa IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Rede WiFi: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/tag1Move", handle_tag1Move);
  server.on("/tag2Move", handle_tag2Move);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("Servidor HTTP iniciado!");
}

void loop() {

  server.handleClient();

  atualizarTempUmid();

  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    Serial.println("Com tag");
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    Serial.println("Leu tag");
    if (tag == "1355610852") {
      dadosOvelha = "Nome: Estrelinha; Peso: 52; DataNasc: 05/10/2020";
      handle_tag1Move();
    } else if (tag == "49919329") {
      dadosOvelha = "Nome: Pintada; Peso: 49; DataNasc: 08/04/2019";
      handle_tag2Move();
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  Serial.print("Tentou ler");
}

void atualizarTempUmid() {
  TemperaturaLida = dht.readTemperature();
  UmidadeLida = dht.readHumidity();
  Serial.println("Temperatura");
  Serial.println(TemperaturaLida);
  Serial.println("Umidade");
  Serial.println(UmidadeLida);
  delay(5000);
}

void handle_OnConnect() { server.send(200, "text/html", SendHTML()); }

void handle_ledon() { server.send(200, "text/html", SendHTML()); }

void handle_tag1Move() {
  if (tag1Status == true) {
    tag1Status = false;
    externoQtd++;
    internoQtd--;
  } else {
    tag1Status = true;
    externoQtd--;
    internoQtd++;
  }
}

void handle_tag2Move() {
  if (tag2Status == true) {
    tag2Status = false;
    externoQtd++;
    internoQtd--;
  } else {
    tag2Status = true;
    externoQtd--;
    internoQtd++;
  }
}

void handle_ledoff() { server.send(200, "text/html", SendHTML()); }

void handle_NotFound() { server.send(404, "text/plain", "Not found"); }

String SendHTML() {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html lang='en'>";
  ptr += "<head>";
  ptr += "<meta charset='UTF-8'>";
  ptr += "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
  ptr +=
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<meta http-equiv='refresh' content='4'>";
  ptr += "<title>Document</title>";
  ptr += "<style>";
  ptr += ".containerGeral {";
  ptr += "height: 100vh;";
  ptr += "display: flex;";
  ptr += "flex-direction: column;";
  ptr += "align-items: center;";
  ptr += "justify-content: center;";
  ptr += "background-color: bisque;";
  ptr += "}";
  ptr += ".divEspacoInterno {";
  ptr += "border: 4px green dotted;";
  ptr += "height: 200px;";
  ptr += "width: 400px;";
  ptr += "display: flex;";
  ptr += "flex-direction: column;";
  ptr += "align-items: center;";
  ptr += "justify-content: center;";
  ptr += "background-color: chocolate;";
  ptr += "}";
  ptr += ".divEspacoExterno {";
  ptr += "border: 2px green solid;";
  ptr += "width: 800px;";
  ptr += "height: 400px;";
  ptr += "display: flex;";
  ptr += "flex-direction: row;";
  ptr += "align-items: flex-end;";
  ptr += "justify-content: center;";
  ptr += "background-color: aquamarine;";
  ptr += "}";
  ptr += "img {";
  ptr += "width: 70px;";
  ptr += "height: 70px;";
  ptr += "}";
  ptr += "p {";
  ptr += "font-size: 22px;";
  ptr += "}";
  ptr += "</style>";
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<div class='containerGeral'>";
  ptr += "<h1>Localização de animais</h1>";
  ptr += "<h2>Total de animais: 2</h2>";
  ptr += "<h2>";
  ptr += dadosOvelha;
  ptr += "</h2>";
  ptr += "<p>Temperatura: ";
  ptr += TemperaturaLida;
  ptr += " C</p>";
  ptr += "<p>Umidade: ";
  ptr += UmidadeLida;
  ptr += " %</p>";
  ptr += "<div class='divEspacoExterno'>";
  int numerob = externoQtd;
  ptr += "<p>Externo: ";
  ptr += numerob;
  ptr += "</p>";
  for (int i = 1; i <= externoQtd; i++) {
    ptr += "<img "
           "src='https://img.icons8.com/external-goofy-solid-kerismaker/96/"
           "000000/external-Nanny-goat-farming-goofy-solid-kerismaker.png' />";
  }
  ptr += "<div class='divEspacoInterno'>";
  int numero = internoQtd;
  ptr += "<p>Interno: ";
  ptr += numero;
  ptr += "</p>";
  for (int i = 1; i <= internoQtd; i++) {
    ptr += "<img "
           "src='https://img.icons8.com/external-goofy-solid-kerismaker/96/"
           "000000/external-Nanny-goat-farming-goofy-solid-kerismaker.png' />";
  }
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "<script>";
  ptr += "var tag1Num = 18611599;";
  ptr += "var tag2Num = 49919329;";
  ptr += "var tag1Status = 'interno';";
  ptr += "var tag1Status = 'interno';";
  ptr += "var apriscoQtd = 2;";
  ptr += "var externoQtd = 0;";
  ptr += "function registrarMovimento() {";
  ptr += "}";
  ptr += "</script>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}