
#include <EEPROM.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include "LowPower.h"
#include <DS3232RTC.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2561_U.h>
#include "DHT.h"
#include <Servo.h>
Servo myservo;


bool ok = LOW;
bool ok2 = LOW;
bool ok3 = LOW;
bool ok4 = LOW;

int posa = 90;
bool n2;
int err_h = 0;

int sended = 0;

bool send1 = HIGH;
bool send2 = HIGH;
bool send3 = HIGH;
volatile bool Muestreo = LOW;
volatile bool DATA_I = LOW;
int error_ee = 0;


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define CE_PIN 9
#define CSN_PIN 10

#define FULL 275
#define DRY 1023
Adafruit_BMP280 bmp;
int G = 0;
int S = 0;
const int wakeUpPin = 2;
const int wakeUpPin2 = 2;
int contador = 0;
int n = contador ;
int SECONDS = 0;
RTC_DS3231 rtc;
byte direccion[5] = {'a', 'e', 'r', 'i', 's'};
byte direccion2[5] = {'s', 'i', 'r', 'e', 'a'};
RF24 radio(CE_PIN, CSN_PIN);
float datosE[8];
float datos[8];
float datos2[8];
float datos3[8];
float datos4[8];


int startM = 0;
int MAX = 0;
int actual = 0;
bool z = false;

union Float_byte
{
  float datoF;
  byte datoB[4];
} unionFB;


union Integer_byte
{
  unsigned int datoI;
  byte datoB[2];
} unionIB;



void setup()
{
  datos[0] = 1;
  datos2[0] = 2;
  datos3[0] = 3;
  datos4[0] = 4;
  for (int c = 1; c < 8; c++) {
    datos3[c] = c;
  }
  for (int c = 1; c < 8; c++) {
    datos4[c] = 0;
  }

  radio.begin();

  //radio.setRetries(15, 15);
  Serial.begin(115200);
  // Serial.println(actual);
  analogReference(INTERNAL);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode( wakeUpPin, INPUT_PULLUP);

  pinMode( 3, INPUT);

  if (!tsl.begin())
  {
    Serial.print("NOT TSL2561");
    while (1);
  }

  if (! rtc.begin()) {
    Serial.println("NOT RTC");
    while (1);
  }

  if (!bmp.begin()) {
    Serial.println(F("NOT BMP280"));
    while (1);
  }
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  if (now.minute() < 10) {
    startM = 1;
  }
  if ((now.minute() >= 10) && (now.minute() < 20)) {
    startM = 2;
  }
  if ((now.minute() >= 20) && (now.minute() < 30)) {
    startM = 3;
  }
  if ((now.minute() >= 30) && (now.minute() < 40)) {
    startM = 4;
  }
  if ((now.minute() >= 40) && (now.minute() < 50)) {
    startM = 5;
  }
  if ((now.minute() >= 50) && (now.minute() <= 59)) {
    startM = 0;
  }
  startM = startM * 10;
  // startM = 27;

  //Tigre :v
  //gorilla :V X2

    

  Serial.print(startM);
  Serial.println();

  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, true);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);
  RTC.setAlarm(ALM1_MATCH_MINUTES, 0, startM, 0, 1);
  RTC.alarm(ALARM_1);
  configureSensor();
  displaySensorDetails();
  dht.begin();
  radio.openWritingPipe(direccion);
  radio.openReadingPipe(1, direccion2);
  radio.startListening();
  PosicionarPanel(now.hour());
  actual = now.hour();

  digitalWrite(14, HIGH);


  for (int x = 0; x < 1000; x++) {
    int i =  analogRead(A3);
    i =  analogRead(A3);

  }

  digitalWrite(14, LOW);

  //  Serial.println(actual);

  Serial.flush();
  MAX = 0;
}

void loop()
{

  attachInterrupt( 0, Boton, FALLING);
  attachInterrupt( 1, INT, FALLING);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);

  //////////////////Inicio del programa

  if (EEPROM.read(1022) == 0) {
    Serial.print("Inicio Exitoso ");
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(' ');
  }

  if (EEPROM.read(1022) > 0) {
    Serial.println(" Errores Detectados ");
    Serial.println(" Borrando Datos ");
    for (int y = 0; y <= 1023; y++) {
      EEPROM.write(y , 0);
    }
  }

  EEPROM.write(1022, 1);
  ///////////Sistema de deteccion de errores activado

  if (DATA_I == HIGH) {
    detachInterrupt(1);

    /* delay(250);
      if(digitalRead(3)==HIGH){
      datos4[1] = 1;

      do
      {
       ok4 = radio.write(datos4, sizeof(datos4));
       MAX++;
       // Serial.println(MAX);
       if (MAX == 500) {
         ok4 = HIGH;
       }
      } while (!ok4);
      MAX = 0;
      }*/


    if ( radio.available() )
    {
      Serial.println("iniciando Test");
      radio.read(datosE, sizeof(datosE));
      if (datosE[0] == 1) {
        delay(2000);
        radio.stopListening();
        //read_EE(32);
        Scan();
        envio_lite();
        Serial.println("Fin del Test");
        radio.startListening();
      }
    }

  }


  if (Muestreo == HIGH) {
    detachInterrupt(0);
    detachInterrupt(1);

    radio.stopListening();

    DateTime now = rtc.now();
    if ( RTC.alarm(ALARM_1) )
    {
      startM += 10;
      if (startM == 60) {
        startM = 0;
      }
      RTC.setAlarm(ALM1_MATCH_MINUTES, 0, startM, 0, 1);
    }


    if (EEPROM.read(0) < 32) {

      Scan();
      if (datos2[4] != actual) {
        PosicionarPanel(now.hour());
        actual = datos2[4];
      }
      write_EE(32); // Guardar en la posicion 33 de 34 todos los datos para un posible reenvio debido a algun error.


      sended = EEPROM.read(1023);

      if (EEPROM.read(0) == 0) {
        enviar();
        Serial.println("Modo Estandar Activo");
        // Serial.println(EEPROM.read(0));
      }

      if (EEPROM.read(0) > 0) {
        write_EE(EEPROM.read(0) - 1);
        error_ee = EEPROM.read(0) + 1;
        EEPROM.write(0, error_ee);
        send_lost_data(EEPROM.read(0));
        Serial.println("Modo Saver activo");
        //   Serial.println(EEPROM.read(0));

      }
    }


    if (EEPROM.read(0) == 32) {
      send_lost_data(EEPROM.read(0));
      Serial.println("Modo Stand By Activo");
    }

    send3 = HIGH;
    radio.startListening();
    delay(100);
  }

  Muestreo = LOW;
  DATA_I = LOW;



  EEPROM.write(1022, 0);
  Serial.print("Datos guardados: ");
  Serial.print(EEPROM.read(0));
  Serial.print(" ");
  Serial.print("Datos guardados enviados: ");
  Serial.println(EEPROM.read(1023));
  Serial.println("");
  Serial.flush();
}

void Boton() {
  Muestreo = HIGH;
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
}

void configureSensor(void)
{
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
}

void PosicionarPanel(int hora) {
  if ((hora < 23) && (hora >= 6)) {
    myservo.attach(8);
    if ((hora >= 6) && (hora < 9)) {
      servo_sweet(0);
    }
    if ((hora >= 9) && (hora < 11)) {

      servo_sweet(65);
    }
    if ((hora >= 11) && (hora < 14)) {

      servo_sweet(90);
    }
    if ((hora >= 14) && (hora < 17)) {

      servo_sweet(115);
    }
    if ((hora >= 17) && (hora < 23)) {

      servo_sweet(180);
    }
    delay(500);
    myservo.detach();
  }
}

int Calibrar(int H, int Batt) {
  int Y = 372 - Batt;
  float A = (Y * 292) / 110;
  float B = (Y * 15) / 110;
  A = 1015 - int(A);
  B = 160 - int (B);
  return (constrain(map(H, A, B, 0, 100), 0, 100));
}

int Calibrar2(int H, int Batt) {
  int Y = 372 - Batt;
  float A = (Y * 292) / 110;
  float B = (Y * 15) / 110;
  A =  830 - int(A);
  B = 428 - int (B);
  return (constrain(map(H, A, B, 0, 100), 0, 100));
}



void INT() {
  DATA_I = HIGH;
}

void servo_sweet(int a) {
  if (a < posa) {
    n2 = 0;
  }
  if (a > posa) {
    n2 = 1;
  }


  if (n2) {
    for (int x = posa; x <= a; x++) {
      myservo.write(x);
      delay(100);
    }
  }
  if (!n2) {
    for (int x = posa; x >= a; x--) {
      myservo.write(x);
      delay(100);
    }
  }

  posa = a;
}

void read_EE(int pos) {
  pos = pos * 30 + 1;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 1);
  unionIB.datoB[1] = EEPROM.read(pos + 2);
  datos2[3] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 3);
  unionIB.datoB[1] = EEPROM.read(pos + 4);
  datos2[2] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 5);
  unionIB.datoB[1] = EEPROM.read(pos + 6);
  datos2[1] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 7);
  unionIB.datoB[1] = EEPROM.read(pos + 8);
  datos2[4] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 9);
  unionIB.datoB[1] = EEPROM.read(pos + 10);
  datos2[5] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 11);
  unionIB.datoB[1] = EEPROM.read(pos + 12);
  datos[2] = unionIB.datoI;
  unionFB.datoF = 0.0;
  unionFB.datoB[0] = EEPROM.read(pos + 13);
  unionFB.datoB[1] = EEPROM.read(pos + 14);
  unionFB.datoB[2] = EEPROM.read(pos + 15);
  unionFB.datoB[3] = EEPROM.read(pos + 16);
  datos[1] = unionFB.datoF;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 17);
  unionIB.datoB[1] = EEPROM.read(pos + 18);
  datos[5] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 19);
  unionIB.datoB[1] = EEPROM.read(pos + 20);
  datos[6] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 21);
  unionIB.datoB[1] = EEPROM.read(pos + 22);
  datos[4] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 23);
  unionIB.datoB[1] = EEPROM.read(pos + 24);
  datos2[7] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 25);
  unionIB.datoB[1] = EEPROM.read(pos + 26);
  datos[3] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 27);
  unionIB.datoB[1] = EEPROM.read(pos + 28);
  datos[7] = unionIB.datoI;
  unionIB.datoI = 0;
  unionIB.datoB[0] = EEPROM.read(pos + 29);
  unionIB.datoB[1] = EEPROM.read(pos + 30);
  datos3[1] = unionIB.datoI;
}

void write_EE(int pos) {
  pos = pos * 30 + 1;
  unionIB.datoI = int(datos2[3]);
  EEPROM.write(pos + 1, unionIB.datoB[0]);
  EEPROM.write(pos + 2, unionIB.datoB[1]);
  unionIB.datoI = int(datos2[2]);
  EEPROM.write(pos + 3, unionIB.datoB[0]);
  EEPROM.write(pos + 4, unionIB.datoB[1]);
  unionIB.datoI = int(datos2[1]);
  EEPROM.write(pos + 5, unionIB.datoB[0]);
  EEPROM.write(pos + 6, unionIB.datoB[1]);
  unionIB.datoI = int(datos2[4]);
  EEPROM.write(pos + 7, unionIB.datoB[0]);
  EEPROM.write(pos + 8, unionIB.datoB[1]);
  unionIB.datoI = int(datos2[5]);
  EEPROM.write(pos + 9, unionIB.datoB[0]);
  EEPROM.write(pos + 10, unionIB.datoB[1]);
  unionIB.datoI = int(datos[2]);
  EEPROM.write(pos + 11, unionIB.datoB[0]);
  EEPROM.write(pos + 12, unionIB.datoB[1]);
  unionFB.datoF =  datos[1];
  EEPROM.write(pos + 13, unionFB.datoB[0]);
  EEPROM.write(pos + 14, unionFB.datoB[1]);
  EEPROM.write(pos + 15, unionFB.datoB[2]);
  EEPROM.write(pos + 16, unionFB.datoB[3]);
  unionIB.datoI = int(datos[5]);
  EEPROM.write(pos + 17, unionIB.datoB[0]);
  EEPROM.write(pos + 18, unionIB.datoB[1]);
  unionIB.datoI = int(datos[6]);
  EEPROM.write(pos + 19, unionIB.datoB[0]);
  EEPROM.write(pos + 20, unionIB.datoB[1]);
  unionIB.datoI = int(datos[4]);
  EEPROM.write(pos + 21, unionIB.datoB[0]);
  EEPROM.write(pos + 22, unionIB.datoB[1]);
  unionIB.datoI = int(datos2[7]);
  EEPROM.write(pos + 23, unionIB.datoB[0]);
  EEPROM.write(pos + 24, unionIB.datoB[1]);
  unionIB.datoI = int(datos[3]);
  EEPROM.write(pos + 25, unionIB.datoB[0]);
  EEPROM.write(pos + 26, unionIB.datoB[1]);
  unionIB.datoI = int(datos[7]);
  EEPROM.write(pos + 27, unionIB.datoB[0]);
  EEPROM.write(pos + 28, unionIB.datoB[1]);
  unionIB.datoI = int(datos3[1]);
  EEPROM.write(pos + 29, unionIB.datoB[0]);
  EEPROM.write(pos + 30, unionIB.datoB[1]);
}

void send_lost_data(int e) {
  e -= 1;
  for (S = sended ; S <= e; S++) {
    err_h = S;
    if (send3) {
      read_EE(S);
      envio_lite();
      delay(1000);
    }
    if ((send1 == HIGH) || (send2 == HIGH) || (send3 == HIGH)) {
      G = EEPROM.read(1023) + 1;
      EEPROM.write(1023, G);

      //   Serial.print("Guardados: "); Serial.println(EEPROM.read(0));
      //   Serial.print("Enviados: "); Serial.println(EEPROM.read(1023));

      delay(10);
      if (EEPROM.read(0) == EEPROM.read(1023)) {
        for (int lol = 0; lol < 1024; lol++) {
          EEPROM.write(lol, 0);
        }
        Serial.println("Todo enviado borrando data ");
      }
      // Serial.print("Exuto al enviar lost data: ");
      // Serial.println(err_h);
    }


    if ((send1 == LOW) || (send2 == LOW) || (send3 == LOW)) {
      //  Serial.print("Error al enviar lost data: ");
      //  Serial.println(err_h);

      //  Serial.print("Guardados: "); Serial.println(EEPROM.read(0));
      //  Serial.print("Enviados: "); Serial.println(EEPROM.read(1023));
    }

  }
}

void envio_lite() {
  do
  {
    ok = radio.write(datos, sizeof(datos));
    send1 = ok;
    MAX++;
    if (MAX == 10) {
      ok = HIGH;
      send1 = LOW;
      send2 = LOW;
      send3 = LOW;
    }
  } while (!ok);
  MAX = 0;



  if (send1) {
    do
    {
      ok2 = radio.write(datos2, sizeof(datos2));
      send2 = ok2;
      MAX++;
      if (MAX == 10) {
        ok2 = HIGH;
        send2 = LOW;
        send3 = LOW;
      }
    } while (!ok2);
  }
  MAX = 0;


  if (send2) {
    do
    {
      ok3 = radio.write(datos3, sizeof(datos3));
      send3 = ok3;
      MAX++;
      if (MAX == 10) {
        ok3 = HIGH;
        send3 = LOW;
      }
    } while (!ok3);
  }
  MAX = 0;
}



void enviar() {
  do
  {
    ok = radio.write(datos, sizeof(datos));
    send1 = ok;
    MAX++;
    if (MAX == 500) {
      //    Serial.println("Error de Envio Cadena 1");

      if (EEPROM.read(0) < 31) {
        error_ee = EEPROM.read(0) + 1;
        EEPROM.write(0, error_ee);
      }


      ok = HIGH;
      send1 = LOW;
      send2 = LOW;
    }
  } while (!ok);
  MAX = 0;



  if (send1) {
    do
    {
      ok2 = radio.write(datos2, sizeof(datos2));
      send2 = ok2;
      MAX++;
      if (MAX == 500) {
        //     Serial.println("Error de Envio Cadena 2");
        if (EEPROM.read(0) < 31) {
          error_ee = EEPROM.read(0) + 1;
          EEPROM.write(0, error_ee);
        }

        ok2 = HIGH;
        send2 = LOW;
      }
    } while (!ok2);
  }
  MAX = 0;


  if (send2) {
    do
    {
      ok3 = radio.write(datos3, sizeof(datos3));
      send3 = ok3;
      MAX++;
      if (MAX == 500) {
        //  Serial.println("Error de Envio Cadena 3");
        if (EEPROM.read(0) < 31) {
          error_ee = EEPROM.read(0) + 1;
          EEPROM.write(0, error_ee);
        }
        ok3 = HIGH;
        send3 = LOW;
      }
    } while (!ok3);
  }
  MAX = 0;
  Serial.flush();

}

void Scan() {
  DateTime now = rtc.now();
  datos[1] = bmp.readPressure();
  datos[2] = bmp.readAltitude(1013.25);
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light)
  {
    datos[3] = event.light;
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");

    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);

  datos[4] = h;
  datos[5] = t;
  datos[6] = hic;
  digitalWrite(14, HIGH);

  int Battery = analogRead(A1);


  long Earth = 0;
  long Air = 0;

  for (int x = 0; x < 1000; x++) {
    float sensorValue2 = analogRead(A2);
    Earth += sensorValue2;
  }
  Earth *= 0.001;


  for (int x = 0; x < 1000; x++) {
    float sensorValue3 = analogRead(A3);
    Air += sensorValue3;
  }
  Air *= 0.001;



  int HT = Calibrar(Earth, Battery);
  int HA = Calibrar2(Air, Battery);
  digitalWrite(14, LOW);

  datos3[1] = HA;
  Battery = map(Battery, 262, 376, 0 , 100);

  datos[7] = Battery;

  datos2[1] = now.year();

  datos2[2] = now.month();

  datos2[3] = now.day();

  datos2[4] = now.hour();

  datos2[5] = now.minute();


  datos2[6] = now.second();

  datos2[7] = HT;

}






