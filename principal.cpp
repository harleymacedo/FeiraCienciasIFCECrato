#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <string> 
#include <stdio.h>
#include <iostream>
#include <sstream>

/* Read RFID Tag with RC522 RFID Reader
 *  Made by miliohm.com
 */
 
#include <SPI.h>
#include <MFRC522.h>


//Modulo wifi
const char* ssid = "brisa-948110";  // SSID Wifi
const char* password = "abc123";  // Senha Wifi
ESP8266WebServer server(80);


constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

//variaveis das tags
int total = 2;
int externoQtd = 0;
int internoQtd = 2;
bool tag1Status = true;
bool tag2Status = true;
//tag1Id = '18611599';
//tag2Id = '49919329';

void setup() {

  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  //Modulo wifi
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

  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "18611599") {
      handle_tag1Move();
    } else if (tag == "49919329") {
      handle_tag2Move();      
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_ledon() {
  server.send(200, "text/html", SendHTML());
}

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

void handle_ledoff() {
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html>";
        ptr += "<html lang='en'>";
        ptr += "<head>";
        ptr += "<meta charset='UTF-8'>";
        ptr += "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
        ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        ptr += "<meta http-equiv='refresh' content='4'>";
        ptr += "<title>Document</title>";
        ptr += "<style>";
        ptr += ".containerGeral {";
        ptr += "text-align: center;";
        ptr += "}";
        ptr += ".divEspacoInterno {";
        ptr += "border: 4px green dotted;";
        ptr += "width: 300px;";
        ptr += "height: 200px;";
        ptr += "left: 500px;";
        ptr += "top: 350px;";
        ptr += "position: absolute;";
        ptr += "}";
        ptr += ".divEspacoExterno {";
        ptr += "border: 2px green solid;";
        ptr += "width: 800px;";
        ptr += "height: 400px;";
        ptr += "left: 200px;";
        ptr += "top: 150px;";
        ptr += "position: absolute;";
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
        ptr += "<div class='divEspacoInterno'>";      
          
        int numero = internoQtd;
        //std::string resultado = "<p>Interno: " + std::to_string(numero) + "</p>";
        ptr += "<p>Interno: ";
        ptr += numero;
        ptr += "</p>";        
        for (int i = 1; i <= internoQtd; i++){
          ptr += "<img src='https://thumbs.dreamstime.com/b/silhueta-da-posi%C3%A7%C3%A3o-cabra-135377248.jpg' />";     
        }
        ptr += "</div>";
        ptr += "<div class='divEspacoExterno'>";
        int numerob = externoQtd;
        //std::string resultado = "<p>Interno: " + std::to_string(numero) + "</p>";
        ptr += "<p>Externo: ";
        ptr += numerob;
        ptr += "</p>";
        for (int i = 1; i <= externoQtd; i++){
          ptr += "<img src='https://thumbs.dreamstime.com/b/silhueta-da-posi%C3%A7%C3%A3o-cabra-135377248.jpg' />";     
        }
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