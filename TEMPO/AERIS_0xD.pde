import co.jimezam.util.Dialogo;

File f;
PrintWriter output;
PFont Nokia;
PFont Nokia2;
PImage Logo; 
PImage Logo2; 
PImage Logo3;

int text = 1;
int text2 = 0;


int valor=0;
int year= year();
int mes = month();
int dia = day();
String data = "";
boolean M = true;
boolean v  = false;
boolean B1 = true;
int C_B1 = 0;
boolean EC_B1 = false;

String DATA = "";
boolean B2 = true;
int C_B2 = 0;
boolean EC_B2 = false;

boolean B3 = true;
int C_B3 = 0;
boolean EC_B3 = false;

boolean B4 = true;
int C_B4 = 0;
boolean EC_B4 = false;

boolean B5 = true;
int C_B5 = 0;
boolean EC_B5 = false;

boolean B6 = true;
int C_B6 = 0;
boolean EC_B6 = false;

boolean B7 = true;
int C_B7 = 0;
boolean EC_B7 = false;

String filename = "geopositions2.txt";
String K = "";

boolean read = false;
boolean read2 = false;
boolean D = false;
boolean D2 = false;
char C=' ';
String L = "";

import processing.serial.*;
Serial myPort;
boolean g = false;


void setup() {

  background(#000000);
  size(500, 300);
  printArray(Serial.list());
  Nokia = createFont("Digitalema", 24);
  Nokia2 = createFont("Nokia Cellphone FC Small", 24);
  Logo = loadImage("Logo.png");
  Logo2 = loadImage("Logo2.png");
  Logo3 = loadImage("Logo3.png");
  myPort = new Serial(this, Serial.list()[32], 9600);
}

void draw() {
  L = fecha();
  D2 = D;
  background(#000000);


  textAlign(CORNER);
  textFont(Nokia);
  textSize(50);
  text("TEMPO", 90, 60);
  textSize(10);
  text("Beta", 410, 60);
  textSize(7);
  text("Desingned for KLIMATO by Dennis Revolorio", 115, 70);
  fill(#FFFFFF);
  rect(100, 90, 90, 130);
  rect(210, 90, 90, 130);
  rect(320, 90, 90, 130);

  rect(230, 240, 150, 40);

  image(Logo, 0, 243);
  image(Logo2, 423, 236);

  if (B1) {
    fill(#000000); 
    rect(110, 100, 70, 20);
    fill(#FFFFFF);
    textSize(8);
    text("A", 142, 113);
  }
  if (!B1) {

    fill(#FFFFFF);
    rect(110, 100, 70, 20);
    fill(#000000); 
    textSize(8);
    text("A", 142, 113);
  }


  if (B2) {
    fill(#000000); 
    rect(110, 190, 70, 20);

    fill(#FFFFFF);
    textSize(8);
    text("B", 142, 205);
  }

  if (!B2) {
    fill(#FFFFFF);
    rect(110, 190, 70, 20);
    fill(#000000); 
    textSize(8);
    text("B", 142, 205);
  }


  if (B3) {
    fill(#000000); 
    rect(220, 100, 70, 20);
    fill(#FFFFFF);
    textSize(8);
    text("A", 252, 113);
  }

  if (!B3) {
    fill(#FFFFFF);
    rect(220, 100, 70, 20);

    fill(#000000);

    textSize(8);
    text("A", 252, 113);
  }






  if (B6) {
    fill(#000000); 
    rect(330, 190, 70, 20);
    fill(#FFFFFF);
    textSize(8);
    text("B", 362, 205);
  }
  if (!B6) {
    fill(#FFFFFF); 
    rect(330, 190, 70, 20);
    fill(#000000);
    textSize(8);
    text("B", 362, 205);
  } 



  if (B5) {
    fill(#000000); 
    rect(330, 100, 70, 20);
    fill(#FFFFFF);
    text("A", 362, 113);
  }
  if (!B5) {
    fill(#FFFFFF);
    rect(330, 100, 70, 20);
    fill(#000000);
    text("A", 362, 113);
  }


  if (B4) {
    fill(#000000); 
    rect(220, 190, 70, 20);
    fill(#FFFFFF);
    textSize(8);
    text("B", 252, 205);
  }
  if (!B4) {
    fill(#FFFFFF);
    rect(220, 190, 70, 20);

    fill(#000000);
    textSize(8);
    text("B", 252, 205);
  }

  if (B7) {
    fill(#000000); 
    rect(240, 250, 130, 20);
    fill(#FFFFFF);
    textSize(8);
    text("Solicitar Archivo", 245, 265);
  }
  if (!B7) {
    fill(#FFFFFF);
    rect(240, 250, 130, 20);

    fill(#000000);
    textSize(8);
    text("Solicitar Archivo", 245, 265);
  }

  fill(#000000);   

  textAlign(CENTER);
  textSize(10);
  text("YEAR", 368, 185);
  text("MONTH", 258, 185);
  text("DAY", 148, 185);
  textFont(Nokia2);


  textSize(25); 

  text(year, 368, 160);
  text(mes, 258, 160);
  text(dia, 148, 160);

  fill(#FFFFFF);  





  /* textAlign(CENTER);
   textSize(15);
   text(valor,360,160);
   text(valor,360,160);*/

  if (EC_B1) {
    C_B1++;
  }
  if (C_B1>7) {
    B1=true;
    C_B1=0;
    EC_B1=false;
  }

  if (EC_B2) {
    C_B2++;
  }
  if (C_B2>7) {
    B2=true;
    C_B2=0;
    EC_B2=false;
  }

  if (EC_B3) {
    C_B3++;
  }
  if (C_B3>7) {
    B3=true;
    C_B3=0;
    EC_B3=false;
  }

  if (EC_B4) {
    C_B4++;
  }
  if (C_B4>7) {
    B4=true;
    C_B4=0;
    EC_B4=false;
  }

  if (EC_B5) {
    C_B5++;
  }
  if (C_B5>7) {
    B5=true;
    C_B5=0;
    EC_B5=false;
  }

  if (EC_B6) {
    C_B6++;
  }
  if (C_B6>7) {
    B6=true;
    C_B6=0;
    EC_B6=false;
  }
  if (EC_B7) {
    C_B7++;
  }
  if (C_B7>7) {
    B7=true;
    C_B7=0;
    EC_B7=false;
  }
  if (D2) {   

    output = createWriter(L);
    output.print(DATA);
    output.flush();
    output.close();
    g=false;
    Dialogo.mostrar("AERIS_0xD", "Archivo Descargado", Dialogo.TIPO_INFORMACION);
    D = false;
    DATA="";
  } 


  fill(#555555, 225);
  if (g) {
       rect(0, 0, 500, 300);
    textSize(15);
    textAlign(CORNER);
 fill(#FFFFFF);
    image(Logo3, 140, 0);
    text("Descargando archivo de KLIMATO desde AERIS",50,235);
       textAlign(CENTER);
         textSize(25);
    switch(text){
     case 1:
   
      text(K + " .",250,270);
     break;
    case 2:
    text(K + " ..",250,270);
     break;
     case 3:
     text(K + " ...",250,270);
     break;
     case 4:
     text(K + " ....",250,270);
     break;
     case 5:
     text(K + " .....",250,270);
     break;  
     case 6:
     text(K + " ......",250,270);
     break; 
      
    }
    text2++;
    if(text2>15){
      text++;
      text2 = 0;
      if(text>6) {text=1;}
    } 
  }
  fill(#FFFFFF);
}

String fecha() {
  K = "K_";
  if (dia<10) {
    K+="0"; 
    K+=dia;
  } else {
    K+=dia;
  }
  if (mes<10) {
    K+="0"; 
    K+=mes;
  } else {
    K+=mes;
  }
  K += year - 2000;
  K += ".csv";
  return(K);
}

void mousePressed() {
if(!g){
  if ((mouseX<180)&&(mouseX>110)&&(mouseY>100)&&(mouseY<120)) {
    EC_B1 = true;
    B1=false;
    dia++;
  }
  if ((mouseX<180)&&(mouseX>110)&&(mouseY>190)&&(mouseY<210)) {
    EC_B2 = true;
    B2=false;
    dia--;
  }
  if ((mouseX<290)&&(mouseX>220)&&(mouseY>100)&&(mouseY<120)) {
    EC_B3 = true;
    B3=false;
    mes++;
  }
  if ((mouseX<290)&&(mouseX>220)&&(mouseY>190)&&(mouseY<210)) {
    EC_B4 = true;
    B4=false;
    mes--;
  }
  if ((mouseX<400)&&(mouseX>330)&&(mouseY>100)&&(mouseY<120)) {
    EC_B5 = true;
    B5=false;
    year++;
  }
  if ((mouseX<400)&&(mouseX>330)&&(mouseY>190)&&(mouseY<210)) {
    EC_B6 = true;
    B6=false;
    year--;
  }
  if ((mouseX<370)&&(mouseX>210)&&(mouseY>250)&&(mouseY<270)) {
    EC_B7 = true;
    B7=false;


    f = new File(sketchPath(fecha()));
    if (f.exists())
    {
      Dialogo.mostrar("AERIS_0xD", "El archivo ya existe", Dialogo.TIPO_ADVERTENCIA);
    } else {
      myPort.write(fecha());
    }
  }}
}

void serialEvent(Serial myPort) {
  C = myPort.readChar();

  if (C=='#') {
    D=true;

    C=' ';
  }
  if (C=='$') { 
    Dialogo.mostrar("AERIS_0xD", "El archivo no existe", Dialogo.TIPO_ERROR);
    C=' ';
  }
  if (C=='!') { 
    g=true;
    C=' ';
    text=0;
  }

  DATA += C;
}

void keyPressed() {
  Dialogo.mostrar("Demostración #1a", "El Archivo no existe en AERIS", Dialogo.TIPO_ERROR);
}
