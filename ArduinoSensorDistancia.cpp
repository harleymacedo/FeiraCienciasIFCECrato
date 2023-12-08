// Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>

// Define os pinos para o trigger e echo
#define pino_trigger 11
#define pino_echo 12

// Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup() {
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
}

void loop() {
  // Le as informacoes do sensor, em cm e pol
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  // Exibe informacoes no serial monitor
  Serial.print("Nível da água: ");
  Serial.print(10 - cmMsec);
  delay(9000);
}
