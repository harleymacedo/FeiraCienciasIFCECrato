//Permissao no Linux para gravar
//sudo usermod -a -G dialout harley
//sudo chmod a+rw /dev/ttyACM0

//INCLUSÃO DAS BIBLIOTECAS
#include <dht.h>
#include <LiquidCrystal.h>

//DEFINIÇÃO DE PINOS
#define pinSensor 7
#define cooler 8
#define umidificador 9

//INTERVALO DE LEITURA
#define intervalo 4000

//CRIANDO VARIAVEIS E INSTANCIANDO OBJETOS
unsigned long delayIntervalo;
dht sensorDHT;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup()
{
  // INICIANDO MONITOR SERIAL  
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(cooler, OUTPUT);
  digitalWrite(cooler, LOW);

  pinMode(umidificador, OUTPUT);
  digitalWrite(umidificador, LOW);
  
  //IMPRIMINDO INFORMAÇÕES SOBRE A BIBLIOTECA
  Serial.print("VERSAO DA BIBLIOTECA: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Status,\tTempo(uS),\tUmidade(%),\tTemperatura(C)");
}

void loop()
{

    if ( (millis() - delayIntervalo) > intervalo ) {
      //LEITURA DOS DADOS
      unsigned long start = micros();
      int chk = sensorDHT.read22(pinSensor);
      unsigned long stop = micros();
  
      // VERIFICA SE HOUVE ERRO
      switch (chk)
      {
      case DHTLIB_OK:
          Serial.print("OK,\t");
          break;
      case DHTLIB_ERROR_CHECKSUM:
          Serial.print("Checksum error,\t");
          break;
      case DHTLIB_ERROR_TIMEOUT:
          Serial.print("Time out error,\t");
          break;
      case DHTLIB_ERROR_CONNECT:
          Serial.print("Connect error,\t");
          break;
      case DHTLIB_ERROR_ACK_L:
          Serial.print("Ack Low error,\t");
          break;
      case DHTLIB_ERROR_ACK_H:
          Serial.print("Ack High error,\t");
          break;
      default:
          Serial.print("Unknown error,\t");
          break;
      }
  
    
      // EXIBINDO DADOS LIDOS
      Serial.print(stop - start);
      Serial.print(", \t\t");
      Serial.print("Umid: ");
      Serial.print(sensorDHT.humidity, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);
      Serial.print(" %");
      Serial.print(",\t\t");
      Serial.print("Temp: ");
      Serial.println(sensorDHT.temperature, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);

      lcd.setCursor(0,0);
      lcd.print(sensorDHT.humidity, 1);
      lcd.print("%");
      lcd.setCursor(0,1);
      lcd.print(sensorDHT.temperature, 1);
      lcd.print("c");

      if (sensorDHT.temperature > 29) {
        digitalWrite(cooler, LOW);
      } else {
        digitalWrite(cooler, HIGH);
      }

      if (sensorDHT.humidity < 65) {
        digitalWrite(umidificador, LOW);
      } else {
        digitalWrite(umidificador, HIGH);
      }

      delayIntervalo = millis();
    };
}
