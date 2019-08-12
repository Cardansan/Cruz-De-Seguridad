//Programa para la Cruz de Seguridad
//NOMENCLATURA: A lo largo de este programa se usan variables como mes o día, cada una referente
//a secciones diferentes del display. La relación es la siguiente:
//"días" - Se referirá al display de 4 dígitos que tiene 3 npx por segmento
//"fecha" - Se referirá a los 229 neopixels que conforman las 31 posibles fechas del Mes
//"mes" - Trata de los dos dígitos con dos neopixels por segmento del inferior del display, arriba del año
//"año" - Trata de los dos dígitos con dos neopixels por segmento del inferior del display, debajo del mes


//#include <Wire.h>
//#include <SPI.h>
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define PIN A10
#define PIN1 A11
//#define NUM_PIXELS 369
#define NUM_PIXELS 229  //cruz
#define NUM_PIXELS1 84  //dias totales

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS1, PIN1, NEO_GRB + NEO_KHZ800);

const int numPixAnio = 28;
const int numPixMes = 28;
const int numPixDias = 84;
const int numPixFechas = 229;

int tecla;
int serialCont=0;
int fechaAccidente;

/*
 // Variables del puerto serial
String valDiasSinAcc= "0001";
String valMes= "0001";
String valAnio= "0001";
int ndiaacc, nmes, nanio, ndiaact;

String valColorAnio= "0127";
String valColorMes= "0127";
String valColorDiaSinAcc= "0127";
String valColorAccidente= "0127";
int coloranio, colormes, colordiaacc;
*/
String dia = "0";
String valFecha = "0001";
String valColor = "0127";
String valDiaActual = "0001";
int nfecha, color, color1, ndiaact;

byte numbers[] = {
  B11101110,    // 0
  B10001000,    // 1
  B01111100,    // 2    5555
  B11011100,    // 3   6    4
  B10011010,    // 4    3333
  B11010110,    // 5   2    0
  B11110110,    // 6    1111
  B10001100,    // 7
  B11111110,    // 8
  B10011110    // 9
};
//Cuántos segmentos ocupa cada número de día del mes
int segmDia[]{0,2,5,5,4,5,6,3,7,6,8,4,7,7,6,7,8,5,9,8,11,7,10,10,9,10,11,8,12,11,11,7};
//Suma total de segmentos iluminados hasta el día actual
int segmDiaTotal[]{0,2,7,12,16,21,27,30,37,43,51,55,62,69,75,82,90,95,104,112,123,130,140,150,159,169,180,188,200,211,222,229};

//----------------------------------------------------- configuraciones
void setup(){
  Serial.begin(115200);

  strip.begin();
  delay(100);
  strip.show();
  strip1.begin();
  delay(100);
  strip1.show();

  Serial.println("Iniciado");
}

//--------------------------------------------------- loop
void loop()
{
    if (Serial.available()>0)
  { //Para checar el puerto serial
     /*serialCont++;
     Serial.print("serialCont: "); Serial.println(serialCont);
     if(serialCont<4)
     {
       valFecha = Serial.readStringUntil(',');// read the serial value
       valColor = Serial.readStringUntil(',');
       nfecha = valFecha.toInt();
       color = valColor.toInt();
       if(serialCont==1) {displayNumAnio(nfecha,color); Serial.print("Anio: "); Serial.println(nfecha);}//Sólo guardamos el número y color
       else if(serialCont==2) {displayNumMes(nfecha,color); Serial.print("Mes: "); Serial.println(nfecha);}//Sólo guardamos el número y color
       else if(serialCont==3) {displayNumDiasSinAcc(nfecha,color); Serial.print("Dias sin Accidentes: ");Serial.println(nfecha);}//Sólo guardamos el número y color
     }
     else if(serialCont==4) {valDiaActual = Serial.readStringUntil(','); ndiaact = valDiaActual.toInt();} //De aquí sólo guardamos el día actual
     else if(serialCont>4 && serialCont<=35)//Todos los casos entre 4 y 35 (31 días)
     {
       valColor = Serial.readStringUntil(',');
       color = valColor.toInt();
       fechaAccidente = serialCont - 4; //Para referirnos a cada día
       displayAccidente(fechaAccidente,color);
     }
     if(serialCont>=35) //Los 31 días más los 4 casos previos
      {
       serialCont=0;//Reseteamos el contador
       strip.show();//Ahora sí mostramos todo al mismo tiempo
     }*/
     //Serial.println("Ingrese dias totales sin accidentes y color (0-4): "); valDiaActual = Serial.readStringUntil(','); ndiaact = valDiaActual.toInt();
     //valColor = Serial.readStringUntil(',');
     //color1 = valColor.toInt();
     //Serial.println(ndiaact);
     //displayNumDiasSinAcc(ndiaact,color1);

     Serial.println("Ingrese Dia del Mes y nivel de Accidente (0,1,2,3,4): ");
     dia = Serial.readStringUntil(','); fechaAccidente = dia.toInt();

     valColor = Serial.readStringUntil(',');
     color = valColor.toInt();
     Serial.println(fechaAccidente); //Serial.print(fechaAccidente);
     Serial.println(color);

     displayAccidente(fechaAccidente,color);
     //ndiaact++;
  }
}


//--------------------------------------------------- displayNumDiasSinAcc---------------------------------------------------------------
void displayNumDiasSinAcc(uint16_t h, uint32_t col) // ARGUMENTOS: Número a mostrar, color para Wheel().
{
  uint16_t firstDigit = h / 1000;
  uint16_t secondDigit = (h % 1000)/100;
  uint16_t thirdDigit = ((h%1000)%100)/ 10;
  uint16_t fourthDigit = ((h%1000)%100)%10;
  //int j = numPixAnio + numPixMes + numPixFechas; //Para la posicion del neopixel.
  int j = 0;  //Para la posicion del neopixel
  int i = 0;

  //---------------------------------------- firstDigit
  for ( i = 0; i < 7; i++)
  {
    if ((numbers[firstDigit] & (1 << 7 - i)) && (firstDigit > 0)) // Que sea diferente de cero
    { //Revisa el byte específico del vector y decide si debe encenderlo de algún color o apagarlo
      strip1.setPixelColor(j + (21 * 3), Wheel(col)); //Le asigna el color o...
      strip1.setPixelColor(j+1 + (21 * 3), Wheel(col));
      strip1.setPixelColor(j+2 + (21 * 3), Wheel(col));
    }
    else
    {
      strip1.setPixelColor(j + (21 * 3 ), 0,0,0,0); //...lo apaga
      strip1.setPixelColor(j+1 + (21 * 3 + 2), 0,0,0);
      strip1.setPixelColor(j+2 + (21 * 3 + 2), 0,0,0);
    }
    j=j+3; //Posicion del Neopixel
  }

  // ---------------------------------------- secondDigit
  j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[secondDigit] & (1 << 7 - i)) && ((secondDigit >= 1) || (firstDigit > 0))) //el cero no se muestra
    {
      strip1.setPixelColor(j + (21*2),Wheel(col));
      strip1.setPixelColor(j+1 + (21*2),Wheel(col));
      strip1.setPixelColor(j+2 + (21*2),Wheel(col));
    }
    else
    {
      strip1.setPixelColor(j + (21*2), 0,0,0);
      strip1.setPixelColor(j+1 + (21*2), 0,0,0);
      strip1.setPixelColor(j+2 + (21*2), 0,0,0);
    }
    j=j+3;
  }
   //-------------------------------------------- thirdDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0))) //el cero no se muestra
    {
      strip1.setPixelColor(j + 21, Wheel(col));
      strip1.setPixelColor(j+1 + 21,Wheel(col));
      strip1.setPixelColor(j+2 + 21,Wheel(col));
    }
    else
    {
      strip1.setPixelColor(j + 21, 0,0,0);
      strip1.setPixelColor(j+1 + 21, 0,0,0);
      strip1.setPixelColor(j+2 + 21, 0,0,0);
    }
    j=j+3;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip1.setPixelColor(j ,Wheel(col));
      strip1.setPixelColor(j+1 , Wheel(col));
      strip1.setPixelColor(j+2 , Wheel(col));
    }
    else
    {
      strip1.setPixelColor(j , 0,0,0);
      strip1.setPixelColor(j+1 , 0,0,0);
      strip1.setPixelColor(j+2 , 0,0,0);
    }
    j=j+3;
  }
   strip1.show();
}

//--------------------------------------------------- displayNumMes----------------------------------------------------
void displayNumMes(uint16_t h, uint32_t col)
{
  uint16_t firstDigit = h / 1000;
  uint16_t secondDigit = (h % 1000)/100;
  uint16_t thirdDigit = ((h%1000)%100)/ 10; //En el año y el mes sólo se muestran los últimos dos dígitos
  uint16_t fourthDigit = ((h%1000)%100)%10;
  int j = numPixAnio;
  int i = 0;
   //-------------------------------------------- thirdDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0)))
    {
      strip.setPixelColor(j + 14, Wheel(col));
      strip.setPixelColor(j+1 + 14,Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + 14, 0,0,0);
      strip.setPixelColor(j+1 + 14, 0,0,0);
    }
    j=j+2;
  }
   // -------------------------------------------- fourthDigit
   j=numPixAnio;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip.setPixelColor(j ,Wheel(col));
      strip.setPixelColor(j+1 , Wheel(col));
    }
    else
    {
      strip.setPixelColor(j , 0,0,0);
      strip.setPixelColor(j+1 , 0,0,0);
    }
    j=j+2;
  }
   //strip.show();
}
//--------------------------------------------------- displayNumAnio----------------------------------------------------
void displayNumAnio(uint16_t h, uint32_t col)
{
  uint16_t firstDigit = h / 1000;
  uint16_t secondDigit = (h % 1000)/100;
  uint16_t thirdDigit = ((h%1000)%100)/ 10; //En el año y el mes sólo se muestran los últimos dos dígitos
  uint16_t fourthDigit = ((h%1000)%100)%10;
  int j = 0;
  int i = 0;

   //-------------------------------------------- thirdDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0)))
    {
      strip.setPixelColor(j + 14, Wheel(col));
      strip.setPixelColor(j+1 + 14,Wheel(col));
    }
    else
    {
      strip.setPixelColor(j + 14, 0,0,0);
      strip.setPixelColor(j+1 + 14, 0,0,0);
    }
    j=j+2;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip.setPixelColor(j ,Wheel(col));
      strip.setPixelColor(j+1 , Wheel(col));
    }
    else
    {
      strip.setPixelColor(j , 0,0,0);
      strip.setPixelColor(j+1 , 0,0,0);
    }
    j=j+2;
  }
   //strip.show();
}
//--------------------------------------------------- displayNumFecha--------------------------------------------------------------
void displayAccidente(uint16_t h, uint32_t col)
{
   uint16_t i;
   /*for( i =  0; i < strip.numPixels(); i++) //
   {
     strip.setPixelColor(i,0,0,0,0);
   }*/
   int omitir = segmDiaTotal[h-1];  //Accede al arreglo segmDiaTotal una posicion anterior a la seleccionada para saber cuantos npx omitir
   int mostrar = segmDia[h];        //Accede a este arreglo para saber el numero de leds a encender posteriormente en el ciclo for
   for (uint16_t i=0; i<mostrar; i++)
   {

    strip.setPixelColor(i+omitir,Wheel(col));
   }
   strip.show();
}
//---------------------------------------------------------------------- apagaPixels
void apagaPixels()
{
    uint16_t i;
    for( i =  0; i < strip.numPixels(); i++) //
    {
      strip.setPixelColor(i,0,0,0);
    }  strip.show();
}

//---------------------------------- Input a value 0 to 255 to get a color value r - g - b - back to r + a number (0-255) for brightness!!
uint32_t Wheel(byte WheelPos)
{
  switch (WheelPos)
  {
    case 0:
      return strip.Color(0,255,0);  //Verde
      break;
    case 1:
      return strip.Color(193,0,255); //Violeta, hay problemas para mostrar el color naranja con los npx
      break;
    case 2:
      return strip.Color(0,0,255);  //Azul
      break;
    case 3:
      return strip.Color(255,195,0);  //Amarillo
      break;
    case 4:
      return strip.Color(255,0,0);  //Rojo
      break;
    case 8:
      return strip.Color(0,0,0);  //Apaga un numero
      break;
    case 9:
      apagaPixels();
            break;
  }
  /*if (WheelPos == 0)
  { //Para el color blanco
    return strip.Color(0, 0, 0, 255);
  }
else {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85 && WheelPos >0 )
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}*/
}
