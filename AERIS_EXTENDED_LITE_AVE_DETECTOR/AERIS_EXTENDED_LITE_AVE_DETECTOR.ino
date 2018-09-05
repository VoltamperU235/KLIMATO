
#include <TimerOne.h>
#include <SPI.h>
#include <SD.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Time.h>
#include <TimeAlarms.h>
#define CE_PIN 9
#define CSN_PIN 10
const byte direccion[5] = {'a', 'e', 'r', 'i', 's'};
const byte direccion2[5] = {'s', 'i', 'r', 'e', 'a'};
RF24 radio(CE_PIN, CSN_PIN);
float datos[8];
float datosE[8];
float Sensor[20];
bool alarma = LOW;
bool alarma2 = LOW;
bool alarma3 = LOW;
bool ok = LOW;
int A = 0;
int B = 0;
int C = 0;
bool reset = LOW;

bool reloj = HIGH;
int alarma_momento = 0;
int aumento = 10;
int actual = 500;
bool alarma_enable = HIGH;
String KLI = "";
String DATA = "";
int dato = 0;
String base = "";
const char coma = ',';
const String encabezado = "F, A, Pa, T, HI, HA, HT, L, B, D1, D2, D3, D4, D5, D6, D7";
const String f = ".csv";
const String f2 = "K_";
int MAX = 0;

File myFile;

void setup()
{
  radio.begin();
  Serial.begin(9600);
  radio.openReadingPipe(1, direccion);
  radio.openWritingPipe(direccion2);
  radio.startListening();
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  digitalWrite(14, LOW);
  if (!SD.begin(4)) {
    return;
  }
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(Avisos); // blinkLED to run every 0.15 seconds
  tone(15, 1000, 500);
  datosE[0] = 1;
}

void loop() {

  if ( radio.available() )
  {
    noInterrupts();
    radio.read(datos, sizeof(datos));

    if (datos[0] == 4) {
      if (datos[1] == 1) {
        alarma_error4();
      //  Serial.println("INT 1 activada");
      }
    }

    if (datos[0] == 1) {
      for (int h = 1; h < 8; h++)
      {
        Sensor[h - 1] = datos[h];
      }
      if ((Sensor[0] != 0) || (Sensor[1] != 0) || (Sensor[3] != 0) || (Sensor[4] != 0) || (Sensor[5] != 0)) {
        if (Sensor[6] <= 10) {
          alarma_error3();
         }
      }}






      if (datos[0] == 2) {
        for (int h = 1; h < 8; h++)
        {
          Sensor[h + 6] = datos[h];
        }

      }

      if (datos[0] == 3) {
        for (int h = 1; h < 8; h++) {
          Sensor[h + 13] = datos[h];
          if (Sensor[14] > 5) {
            alarma_error3();
           // Serial.println("Error Detectado lluvia");
     
            
          }
        }

        if (Sensor[11] != actual) {

          if (reloj) {
            setTime(Sensor[10], Sensor[11], Sensor[12], Sensor[9], Sensor[8], Sensor[7]);
            reloj = LOW;
          }

          Check_name(Sensor[7], Sensor[8], Sensor[9]);

          /////////////////////////////////////////////////////////////////////////////////////////////

          if (!SD.exists(KLI))
          {
            myFile = SD.open(KLI, FILE_WRITE);
            if (myFile) {
              delay(50);
              myFile.println(encabezado);
              myFile.close();

            } else {
            }
          }

          myFile = SD.open(KLI, FILE_WRITE);//abrimos  el archivo
          if (myFile) {
            myFile.print(int(Sensor[9]));
            myFile.print("/");
            myFile.print(int(Sensor[8]));
            myFile.print("/");
            myFile.print(int(Sensor[7]));
            myFile.print(" ");
            myFile.print(int(Sensor[10]));
            myFile.print(":");
            myFile.print(int(Sensor[11]));


            //finaliza la fecha y hora
            myFile.print(coma);
            myFile.print(int(Sensor[1]));
            myFile.print(coma);
            myFile.print(Sensor[0]);
            myFile.print(coma);
            myFile.print(int(Sensor[4]));
            myFile.print(coma);
            myFile.print(Sensor[5]);
            myFile.print(coma);
            myFile.print(int(Sensor[3]));
            myFile.print(coma);
            myFile.print(int(Sensor[13]));
            myFile.print(coma);
            myFile.print(int(Sensor[2]));
            myFile.print(coma);
            myFile.print(int(Sensor[6]));
            myFile.print(coma);
            myFile.print(int(Sensor[14]));
            myFile.print(coma);
            myFile.print(int(Sensor[15]));
            myFile.print(coma);
            myFile.print(int(Sensor[16]));
            myFile.print(coma);
            myFile.print(int(Sensor[17]));
            myFile.print(coma);
            myFile.print(int(Sensor[18]));
            myFile.print(coma);
            myFile.print(int(Sensor[19]));
            myFile.print(coma);
            myFile.println(int(Sensor[20]));


            myFile.close();


            actual = Sensor[11];

          } else {

          }
          SET_ALARM(minute());


          //////////////////////////////////////////////////////////////////////////////////////////////////////7

        }
      }


    }



    if (Serial.available()) {
      char c = Serial.read();
      DATA += c;
      dato++;

      if(c == 'T'){test();}

      if (dato == 12) {
        base = DATA;
        DATA = "";
        dato = 0;

        if (SD.exists(base))
        {
          myFile = SD.open(base);//abrimos  el archivo
          if (myFile) {
            Serial.print("!");
            while (myFile.available()) {
              Serial.write(myFile.read());
            }
            Serial.print("#");
            reset = HIGH;
            myFile.close();
          } else {
            Serial.print("¿");
          }

        }

        else {
          alarma_error2();
          Serial.print("$");
        }
      }


    }

    if (!reloj) {
      READ_ALARM();
    }

    if (reset) {
      Serial.flush();
      digitalWrite(17, HIGH);
    }
    interrupts();

  
}

void Avisos(void)
{
  if (A < 6) {
    if (alarma) {
      tone(15, 1000, 700);
      A++;
    }
  }

  if (B < 12) {
    if (alarma2) {
      tone(15, 4000, 200);
      B++;
    }
  }

  if (C < 3) {
    if (alarma3) {
      tone(15, 300, 800);
      C++;
    }
  }
}




void SET_ALARM(int val) {
  alarma_momento = val + 15;
  if (alarma_momento > 59) {
    alarma_momento -= 60;
  }
  alarma_enable = HIGH;
}

void READ_ALARM() {
  if (alarma_enable) {
    if (alarma_momento == minute()) {
      alarma_error1();
      radio.stopListening();
      do
      {
        ok = radio.write(datosE, sizeof(datosE));

        MAX++;
        if (MAX == 500) {
          ok = HIGH;
        }
      } while (!ok);
      MAX = 0;
      radio.startListening();
    }
  }
}

void Check_name(int y, int m, int d) {
  KLI = f2;
  if (d < 10) {
    KLI += "0";
  }
  KLI += String(d);
  if (m < 10) {
    KLI += "0";
  }
  KLI += String(m);
  KLI += String((y - 2000));
  KLI += f;
}

void alarma_error1() {
  alarma = HIGH;
  A = 0;
  alarma_enable = LOW;

}

void alarma_error2() {
  alarma = HIGH;
  A = 5;
  alarma_enable = LOW;

}

void alarma_error3() {
  alarma2 = HIGH;
  B = 0;
}

void alarma_error4() {
  alarma3 = HIGH;
  C = 0;
}

void test(){
  radio.stopListening();
      do
      {
        ok = radio.write(datosE, sizeof(datosE));

        MAX++;
        if (MAX == 500) {
          ok = HIGH;
        }
      } while (!ok);
      MAX = 0;
      radio.startListening();
      Serial.println("Prueba realizada");
}

