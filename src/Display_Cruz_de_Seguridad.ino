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
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>

#define PIN D0
#define PIN1 D1
#define PIN2 D2
#define PIN3 D3
#define Brightness 255
//#define NUM_PIXELS 369
#define NUM_PIXELS 229  //cruz
#define NUM_PIXELS1 84  //dias totales
#define NUM_PIXELS2 28 //Mes
#define NUM_PIXELS3 28 //año

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS2, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_PIXELS3, PIN3, NEO_GRB + NEO_KHZ800);

const int numPixAnio = 28;
const int numPixMes = 28;
const int numPixDias = 84;
const int numPixFechas = 229;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const char WiFiAPPSK[] = "d1spl4y4.0";  //CONTRASEÑA
const char ssid[] = "Display_de_Seguridad"; // NOMBRE DE LA RED

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
String valMes = "00";
String valAnio = "00";
int nfecha, color, color1, ndiaact, tecla, mes, anio;
int newmostrar, newOmitir;

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
//int newsegmDia[]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Socket.send('Connect ' + new Date());
char webpage[] PROGMEM = R"=====(
<html>
<head>
  <script>
    var Socket;
    function init()
    {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/',['arduino']);
      Socket.onopen = function ()
      {
      }
      Socket.onmessage = function(event)
      {
        document.getElementById("rxConsole").value += event.data;
      }
    }
    function sendTextIP()
    {
      Socket.send("IP:"+document.getElementById("txIP").value);
      document.getElementById("txIP").value = "";
    }
    function sendTextSSID()
    {
      Socket.send("SSID:"+document.getElementById("txSSID").value);
      document.getElementById("txSSID").value = "";
    }
    function sendTextPswd()
    {
      Socket.send("PSWD:"+document.getElementById("txPswd").value);
      document.getElementById("txPswd").value = "";
    }
    function sendTextNum1()
    {
      Socket.send("f"+document.getElementById("txNum1").value);
      document.getElementById("txNum1").value = "";
    }
    function sendTextCol1()
    {
      Socket.send("+"+document.getElementById("txCol1").value);
      document.getElementById("txCol1").value = "";
    }
    function sendTextNum2()
    {
      Socket.send("t"+document.getElementById("txNum2").value);
      document.getElementById("txNum2").value = "";
    }
    function sendTextCol2()
    {
      Socket.send("-"+document.getElementById("txCol2").value);
      document.getElementById("txCol2").value = "";
    }
    function sendTextNum3()
    {
      Socket.send("m"+document.getElementById("txNum3").value);
      document.getElementById("txNum3").value = "";
    }
    function sendTextCol3()
    {
      Socket.send("?"+document.getElementById("txCol3").value);
      document.getElementById("txCol3").value = "";
    }
    function sendTextNum4()
    {
      Socket.send("a"+document.getElementById("txNum4").value);
      document.getElementById("txNum4").value = "";
    }
    function sendTextCol4()
    {
      Socket.send("_"+document.getElementById("txCol4").value);
      document.getElementById("txCol4").value = "";
    }
    function sendTextNum5()
    {
      Socket.send("s"+document.getElementById("txNum5").value);
      document.getElementById("txNum5").value = "";
    }
    function sendTextCol5()
    {
      Socket.send("_"+document.getElementById("txCol5").value);
      document.getElementById("txCol5").value = "";
    }
</script>
</head>
<body onload="javascript:init()">
  <div>
    <textarea id="rxConsole"></textarea>
  </div>
  <hr/>
  <div>
    <br/> IP: <input type="text" id="txIP" onkeydown="if(event.keyCode == 13) sendTextIP();" />
  </div>
  <div>
    <br/>SSID: <input type="text" id="txSSID" onkeydown="if(event.keyCode == 13) sendTextSSID();" />
  </div>
  <div>
    <br/>PASSWORD:<input type="text" id="txPswd" onkeydown="if(event.keyCode == 13) sendTextPswd();" />
  </div>
  <div>
    <br/>NUMERO 1: <input type="text" id="txNum1" onkeydown="if(event.keyCode == 13) sendTextNum1();" />
  </div>
  <div>
    <br/>COLOR 1: <input type="text" id="txCol1" onkeydown="if(event.keyCode == 13) sendTextCol1();" />
  </div>
  <div>
    <br/>NUMERO 2: <input type="text" id="txNum2" onkeydown="if(event.keyCode == 13) sendTextNum2();" />
  </div>
  <div>
    <br/>COLOR 2: <input type="text" id="txCol2" onkeydown="if(event.keyCode == 13) sendTextCol2();" />
  </div>
  <div>
    <br/>NUMERO 3: <input type="text" id="txNum3" onkeydown="if(event.keyCode == 13) sendTextNum3();" />
  </div>
  <div>
    <br/>COLOR 3: <input type="text" id="txCol3" onkeydown="if(event.keyCode == 13) sendTextCol3();" />
  </div>
  <div>
    <br/>NUMERO 4: <input type="text" id="txNum4" onkeydown="if(event.keyCode == 13) sendTextNum4();" />
  </div>
  <div>
    <br/>COLOR 4: <input type="text" id="txCol4" onkeydown="if(event.keyCode == 13) sendTextCol4();" />
  </div>
  <div>
    <br/>NUMERO 5: <input type="text" id="txNum5" onkeydown="if(event.keyCode == 13) sendTextNum5();" />
  </div>
  <div>
    <br/>COLOR 5: <input type="text" id="txCol5" onkeydown="if(event.keyCode == 13) sendTextCol5();" />
  </div>
  <hr/>
</body>
</html>
)=====";

//------------------------------------------------------------ setupWiFi
void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, WiFiAPPSK);
  Serial.print("\n &IP Address: ");
  Serial.println(WiFi.softAPIP());
}
//-------------------------------------------------------------webSocketEvent
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch(type)
    {
      case WStype_TEXT:
        for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
        Serial.println("");
      break;

      case WStype_DISCONNECTED:
            //Serial.printf("[%u] Disconnected\n", num);
      break;


     case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            //Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            // send message to client
            webSocket.sendTXT(num, "Setup Iniciado\n");
        }
      break;
    }
}

//----------------------------------------------------- configuraciones
void setup(){
  Serial.begin(115200);
  strip.setBrightness(Brightness);
  strip.begin();
  delay(100);
  strip.show();
  strip1.begin();
  delay(100);
  strip1.show();
  strip2.begin();
  delay(100);
  strip2.show();
  strip3.begin();
  delay(100);
  strip3.show();

  //Serial.print("\nSetting up... ");
  setupWiFi();
  delay(1000);//wait for a second
  if(MDNS.begin("esp8266"))
  {
    //Serial.println("\nMDNS responder started");
  }

  // handle index
  server.on("/", []()
  {
    // send index.html
    server.send_P(200, "text/html", webpage);
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
  webSocket.broadcastTXT("Display Cruz de Seguridad Industria 4.0\n");


  Serial.println("Iniciado");
}

//--------------------------------------------------- loop
void loop()
{
  webSocket.loop();
  server.handleClient();
  if (Serial.available()>0)
  { //Para checar el puerto serial
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
    tecla = Serial.read();  //Lee el primer caracter de la cadena
    switch (tecla)
    {
      case 'f':   //Fecha
      //delay(1000);
      //Serial.println("Ingrese Dia del Mes y nivel de Accidente (0,1,2,3,4): ");
      dia = Serial.readStringUntil(','); fechaAccidente = dia.toInt();
      valColor = Serial.readStringUntil(','); color = valColor.toInt();
      //Serial.println(fechaAccidente); //Serial.print(fechaAccidente);
      //Serial.println(color);
      displayAccidente(fechaAccidente,color);
      break;

      case 't':   //Total
      //delay(1000);
      //Serial.println("Ingrese dias totales sin accidentes y color (0-255): ");
      valDiaActual = Serial.readStringUntil(','); ndiaact = valDiaActual.toInt();
      valColor = Serial.readStringUntil(','); color1 = valColor.toInt();
      //Serial.println(ndiaact);
      displayNumDiasSinAcc(ndiaact,color1);
      break;

      case 'm':   //Mes
      //delay(1000);
      //Serial.println("Ingrese el mes a mostrar y color (0-255): ");
      valMes = Serial.readStringUntil(','); mes = valMes.toInt();
      valColor = Serial.readStringUntil(','); color1 = valColor.toInt();
      displayNumMes(mes,color1);
      break;

      case 'a':   //Año
      //delay(1000);
      //Serial.println("Ingrese el anio a mostrar y color (0-255): ");
      valAnio = Serial.readStringUntil(','); anio = valAnio.toInt();
      valColor = Serial.readStringUntil(','); color1 = valColor.toInt();
      displayNumAnio(anio,color1);
      break;

      case 's':   //Shutdown
      //delay(1000);
      apagaPixels();
      break;
  }

     //strip1.setPixelColor(strip.numPixels(), 0, 250, 0);
     //ndiaact++;
}
  /*for(int i =  0; i < 84; i++) //
  {
    strip2.setPixelColor(i,0,255,0);
  }
  strip2.show();*/
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
  int j = 0;//numPixAnio;
  int i = 0;
   //-------------------------------------------- thirdDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if ((numbers[thirdDigit] & (1 << 7 - i)) && ((thirdDigit >= 1) || (secondDigit > 0) || (firstDigit > 0)))
    {
      strip2.setPixelColor(j + 14, Wheel(col));
      strip2.setPixelColor(j+1 + 14,Wheel(col));
    }
    else
    {
      strip2.setPixelColor(j + 14, 0,0,0);
      strip2.setPixelColor(j+1 + 14, 0,0,0);
    }
    j=j+2;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip2.setPixelColor(j ,Wheel(col));
      strip2.setPixelColor(j+1 , Wheel(col));
    }
    else
    {
      strip2.setPixelColor(j , 0,0,0);
      strip2.setPixelColor(j+1 , 0,0,0);
    }
    j=j+2;
  }
   strip2.show();
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
      strip3.setPixelColor(j + 14, Wheel(col));
      strip3.setPixelColor(j+1 + 14,Wheel(col));
    }
    else
    {
      strip3.setPixelColor(j + 14, 0,0,0);
      strip3.setPixelColor(j+1 + 14, 0,0,0);
    }
    j=j+2;
  }
   // -------------------------------------------- fourthDigit
   j=0;
  for (i = 0; i < 7; i++)
  {
    if (numbers[fourthDigit] & (1 << 7 - i))
    {
      strip3.setPixelColor(j ,Wheel(col));
      strip3.setPixelColor(j+1 , Wheel(col));
    }
    else
    {
      strip3.setPixelColor(j , 0,0,0);
      strip3.setPixelColor(j+1 , 0,0,0);
    }
    j=j+2;
  }
   strip3.show();
}
//--------------------------------------------------- displayNumFecha--------------------------------------------------------------
void displayAccidente(uint16_t h, uint32_t col)
{
   uint16_t i;
   int omitir = segmDiaTotal[h-1];  //Accede al arreglo segmDiaTotal una posicion anterior a la seleccionada para saber cuantos npx omitir
   int mostrar = segmDia[h];        //Accede a este arreglo para saber el numero de leds a encender posteriormente en el ciclo for
   for (uint16_t i=0; i<mostrar; i++)
   {
    strip.setPixelColor(i+omitir, NivelAccidente(col));
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
      strip1.setPixelColor(i,0,0,0);
      strip2.setPixelColor(i,0,0,0);
      strip3.setPixelColor(i,0,0,0);
    }
    strip.show();
    strip1.show();
    strip2.show();
    strip3.show();
}

uint32_t NivelAccidente(byte pos)
{
  switch (pos)
  {
    case 0:
      return strip.Color(0,255,0);  //Verde
      break;
    case 1:
      return strip.Color(77,38,0); //Violeta, hay problemas para mostrar el color naranja con los npx [naranja = ()]violeta = 193,0,255
      break;
    case 2:
      return strip.Color(0,100,255);  //Azul
      break;
    case 3:
      return strip.Color(255,255,0);  //Amarillo255,195,0
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
}

//---------------------------------- Input a value 0 to 255 to get a color value r - g - b - back to r + a number (0-255) for brightness!!
uint32_t Wheel(byte WheelPos)
{


  if (WheelPos == 0)
  { //Para el color blanco
    return strip.Color(255, 255, 255);
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
}
}
