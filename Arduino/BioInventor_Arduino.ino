#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>  
#include <LCD.h> 
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#define DHT11_PIN 8
#include "HX711.h"

dht DHT;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);
HX711 balanza;
HX711 balanza2;

const int DOUT = A1;
const int CLK = A0;
const int DOUT2 = A3;
const int CLK2 = A2;

float temp;
float hum;
float peso1;
float peso1_1;
float peso2;
float peso2_2;
int pfizer;
int astrazeneca;
int janssen;
int sputnik;

void setup() {
  Serial.begin(9600);
  DHT.read11(DHT11_PIN);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  lcd.setBacklightPin(3,POSITIVE); 
  lcd.setBacklight(HIGH); 
  lcd.begin(16, 2);    
  lcd.clear();
  lcd.setCursor(1, 0);  
  lcd.print("VACUNAS SALVAN");
  lcd.setCursor(5,1);
  lcd.print("VIDAS");
  delay(5000);
  lcd.clear();   
  mlx.begin();
  balanza.begin(DOUT, CLK);
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  balanza.set_scale(910.362615384); 
  balanza.tare(20);

  balanza2.begin(DOUT2, CLK2);
  Serial.println(balanza2.read());
  balanza2.set_scale(898.9207142857);
  balanza2.tare(20); 
}

void loop() {
  temp = mlx.readObjectTempC();
  //Serial.println(mlx.readObjectTempC());}
  hum = DHT.humidity;
  peso1_1 = balanza.get_units(20),3;
  peso1 = abs(peso1_1);
  peso2_2 = balanza2.get_units(20),3;
  peso2 = abs(peso2_2);

  if (peso1 >= 605){
    pfizer = 2;
    astrazeneca = 2;
  }
  else if (peso1 >= 430 && peso1 <= 485){
    pfizer = 2;
    astrazeneca = 1;
  }
  else if (peso1 >= 390 && peso1 <= 430){
    pfizer = 1;
    astrazeneca = 2;
  }
  else if ((peso1 >= 255 && peso1 >= 265) && (peso1 >= 330 && peso1 <= 364)){
    pfizer = 2;
    astrazeneca = 0;
  }
  else if (peso1 >= 290 && peso1 <= 305){
    pfizer = 1;
    astrazeneca = 1;
  }
  else if (peso1 >= 185 && peso1 <= 250){
    pfizer = 0;
    astrazeneca = 2;
  }
  else if (peso1 >= 130 && peso1 <= 182){
    pfizer = 1;
    astrazeneca = 0;
  }
  else if (peso1 >= 90 && peso1 <= 127){
    pfizer = 0;
    astrazeneca = 1;
  }
  else if (peso1 < 10){
    pfizer = 0;
    astrazeneca = 0;
  }
  
//PESO2
  if (peso2 >= 600){
    janssen = 2;
    sputnik = 2;
  }
  else if (peso2 >= 450 && peso2 <= 461){
    janssen = 1;
    sputnik = 2;
  }
  else if (peso2 >= 440 && peso2 <= 453){
    janssen = 2;
    sputnik = 1;
  }
  else if (peso2 >= 305 && peso2 <= 320){
    janssen = 0;
    sputnik = 2;
  }
  else if ((peso2 >= 220 && peso2 >= 240) && (peso2 >= 285 && peso2 <= 300)){
    janssen = 2;
    sputnik = 0;
  }
  else if (peso2 >= 140 && peso2 <= 153){
    janssen = 1;
    sputnik = 0;
  }
  else if (peso2 >= 150 && peso2 <= 160){
    janssen = 0;
    sputnik = 1;
  }
  else if (peso2 >= 298  && peso2 <= 305){
    janssen = 1;
    sputnik = 1;
  }
  else if (peso2 < 10){
    janssen = 0;
    sputnik = 0;
  }
  
  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(pfizer);
  Serial.print(",");
  Serial.print(astrazeneca);
  Serial.print(",");
  Serial.print(janssen);
  Serial.print(",");
  Serial.print(sputnik);
  
  if (temp > 30 || hum > 75) {
    if (temp > 30 && hum > 75) {
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      delay(500);
      digitalWrite(11, LOW);
      delay(500);
      lcd.clear();
      lcd.setCursor(2, 0);  
      lcd.print("ALERTA TEMP Y ");
      lcd.setCursor(3, 1);
      lcd.print("HUM ELEVADA");
      delay(3000);
    }
    else if (hum > 75) {
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(11, HIGH);
      delay(500);
      digitalWrite(11, LOW);
      delay(500);
      lcd.clear();
      lcd.setCursor(4, 0);  
      lcd.print("HUMEDAD");
      lcd.setCursor(4, 1);
      lcd.print("ELEVADA");
      delay(3000);
    }
    else if (temp > 30 ){
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      lcd.clear ();
      lcd.setCursor(2, 0);  
      lcd.print("TEMPERATURA");
      lcd.setCursor(4, 1);
      lcd.print("ELEVADA");
      delay(3000);  
    }
  } 
  else if (temp < 30 || hum < 75) {
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    lcd.setCursor(0, 0);  
    lcd.print("Temp: ");
    lcd.print(temp);  
    lcd.print(" C"); 
    digitalWrite(11, HIGH);
    lcd.setCursor(0, 1);  
    lcd.print("Hume: ");
    lcd.print(hum);  
    lcd.print(" %"); 
    delay(500);
  } 
}
