#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <dht.h> //Biblioteca do sensor de temperatura

const int pinoDHT11 = A2; //PINO ANALÓGICO UTILIZADO PELO DHT11
dht DHT; //VARIÁVEL DO TIPO DHT

void setup() {
  Serial.begin(9600); //INICIALIZA A SERIAL
  Serial.print("Vai comecar a impressao \n");
  pinMode(6, OUTPUT); //Pino do ventilador
  pinMode(4, OUTPUT); // Pino do umidificador
}

void loop() {
  DHT.read11(pinoDHT11); //LÊ AS INFORMAÇÕES DO SENSOR
  Serial.print("Umidade: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(DHT.humidity); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
  Serial.print("%"); //ESCREVE O TEXTO EM SEGUIDA
  Serial.print(" / Temperatura: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(DHT.temperature, 0); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
  Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL
  if (DHT.temperature >= 30) { // Valor > 29
    digitalWrite(6, LOW); //Liga o Ventilador
    Serial.println("Estrou no IF da temperatura");
  } else {
    digitalWrite(6, HIGH); //Desliga o Ventilador
    Serial.println("Estrou no ELSE da temperatura");
  }
  if (DHT.humidity <= 60) { // Valor < 65
    digitalWrite(4, LOW); //Liga o umidificador
    Serial.println("Estrou no IF da umidade");
  } else {
    digitalWrite(4, HIGH); //Desliga o umidificador
    Serial.println("Estrou no ELSE da umidade");    
  } 
  delay(8000);
}