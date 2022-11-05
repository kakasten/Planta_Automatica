/*Feito por Luiz para o Projeto SA no Senai
04/11/2022*/
 
#include <Wire.h>               //Biblioteca utilizada gerenciar a comunicação entre dispositivos através do protocolo I2C
#include <LiquidCrystal_I2C.h>  //Biblioteca controlar display 16x2 através do I2C
#include <OneWire.h>
#include <DallasTemperature.h>
 
int valor_analogico;
using namespace std;
 
#define pino_sinal_analogico A0
#define LDR A1
#define POT A2
#define Led 3
#define DS18B20 7  //DEFINE O PINO DIGITAL UTILIZADO PELO SENSOR
 
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Cria o objeto lcd
OneWire ourWire(DS18B20);            //CONFIGURA UMA INST NCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire);
 
void setup() {
 
  IniciaPortas();
  IniciaLcd();
 
  sensors.begin();
  Serial.begin(9600);
 
  //COLOCANDO NOMES FIXOS
  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.setCursor(9, 0);
  lcd.print("LUZ:");
  lcd.setCursor(0, 1);
  lcd.print("RESERVA: ");
}
void loop() {
 
  SensorLuz();
  SensorAgua();
  SensorTemp();
 
  valor_analogico = analogRead(pino_sinal_analogico);
  Serial.print(valor_analogico);
}
 
void IniciaPortas() {
  pinMode(Led, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(POT, INPUT);
  pinMode(pino_sinal_analogico, INPUT);
}
 
void IniciaLcd() {
  lcd.init();       //Inicializa a comunicação com o display já conectado
  lcd.clear();      //Limpa a tela do display
  lcd.backlight();  //Aciona a luz de fundo do display
}
 
void SensorLuz() {
  int Valor_POT = 0;
  int Valor_LDR = 0;
  Valor_LDR = 35 * (analogRead(LDR));
  Valor_POT = analogRead(POT);
 
  Serial.println(Valor_POT);
  Serial.println(Valor_LDR);
  delay(500);
 
  if (Valor_LDR < Valor_POT) {
    digitalWrite(Led, HIGH);
    delay(500);
    lcd.setCursor(13, 0);
    lcd.print("ON");
  }
  if (Valor_LDR > Valor_POT) {
    digitalWrite(Led, LOW);
    delay(500);
    lcd.setCursor(13, 0);
    lcd.print("OFF");
  }
}
 
void SensorTemp() {
  sensors.requestTemperatures();  //SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR
  lcd.setCursor(5, 0);
  lcd.print(int(sensors.getTempCByIndex(0)));  //IMPRIME NO LCD O VALOR DE TEMPERATURA MEDIDO
  delay(250);
}
 
void SensorAgua() {
  lcd.setCursor(0, 7);
 
  //Reservatório Cheio
  if (valor_analogico > 0 && valor_analogico < 560) {
    lcd.print("CHEIO");
  }
  //Reservatório quase Vazio
  if (valor_analogico > 560 && valor_analogico < 750) {
    lcd.print("MEDIA");
  }
  //Reservatório Vazio
  else {
    lcd.print("VAZIO");
  }
}
