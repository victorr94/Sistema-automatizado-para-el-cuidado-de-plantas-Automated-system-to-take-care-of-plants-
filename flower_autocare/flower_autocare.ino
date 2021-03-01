/* Es necesario indicar la hora y fecha actuales antes de cargar el programa a través de la función setTime() */

// Se importan las librerías necesarias para el Servidor Web
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#include <TimeLib.h>      //Librería para configurar fecha y hora
#include <Time.h>         //Librería para configurar fecha y hora
#include <DHT.h>          //Librería sensor DHT 11 (Tª y humedad aire)
#define DHTPIN 15         //Pin digital donde se conecta el sensor DHT 11
#define DHTTYPE DHT11     //Se elige el tipo de sensor DHT 11
DHT dht(DHTPIN, DHTTYPE); //Se inicializa el sensor DHT11

#include <Servo_ESP32.h> //Librería para el servomotor de la herramienta para eliminar malas hierbas

#define IN1 21 //Pin para el driver motor del selector de herramienta
#define IN2 19 //Pin para el driver motor del selector de herramienta
#define IN3 18 //Pin para el driver motor del selector de herramienta
#define IN4 5  //Pin para el driver motor del selector de herramienta

#define RXD2 16 //Pin comunicación serie del ESP32 con Arduino UNO
#define TXD2 17 //Pin comunicación serie del ESP32 con Arduino UNO

// Se incorporan las credenciales de la red Wi-Fi
const char *ssid = "********";
const char *password = "********";

float airhumidity = 40;    //Variable donde se guarda el valor de la humedad el aire
float temperature = 20;    //Variable donde se guarda el valor de la temperatura ambiente
int soilhumidity = 30;     //Variable donde se guarda el valor de la humedad del suelo
int LDRvalue = 0;          //Variable donde se almacena el valor del LDR
int fan = 12;              //Pin para activar ventilador (mediante relé)
int LEDpanel = 27;         //Pin para activar panel LED (mediante relé) cambio 2 por 27
int LDR = 4;               //Pin analógico para LDR cambio
int waterpump = 14;        //Pin para activar bomba de agua (mediante relé)
bool n1 = false;           //Contador para la activación/desactivación Ventilador
bool n2 = false;           //Contador para la activación/desactivación panel LED
bool measurementt = false; //Variable para la activación del proceso "measurement"
bool removeweeds = false;  //Variable para la activación del proceso "remove weeds"
bool watering = false;
static const int servoPin = 13;         //Pin 13 para mover herramienta que elimina malas hierbas (remove weeds)
Servo_ESP32 servo1;                     //Declaro un objeto servo1
byte inicialhour = 10, inicialmin = 10; //Hora a la que se activa el panel LED si no hay suficiente luz natural
byte finalhour = 18, finalmin = 10;     //Hora a la que se desconecta el panel LED si no hay suficiente luz natural
int minutes = 0;                        //Variable donde se guardan los minutos que han pasado para hacer una medición de la temperatura y humedad cada 30 min
int hor = 0;                            //Variable donde se guarda la hora actual, sirve para comparar en procesos que se ejecutan cada hora
int daily = 0;                          //Variable donde se guarda el día actual, sirve para comparar en procesos diarios
int dailyweeds = 0;                     //Variable para hacer proceso de eliminar malas hierbas cada tres días
AsyncWebServer server(80);              //Se crea un objeto AsyncWebServer en el puerto 80

//Variables para controlar el motor paso a paso del selector de herramienta (Paso [ 8 ][ 4 ] , steps_left , Steps)
int Paso[8][4] =  //Matriz de pasos (se utiliza configuración de medios pasos)
{ {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};
int total_steps = 683;    //Número de pasos que se van a dar en total. 683 pasos para seleccionar la primera herramienta (1 vuelta = 4095 pasos)
boolean Direction = true; //Dirección = 1
int Actual_Steps = 0;     //Variable que donde se guardan los pasos actuales (referenciándolos a la matriz Paso[8][4])

void setup()
{
  pinMode(IN1, OUTPUT);          //Pin como salida para el driver motor del selector de herramienta
  pinMode(IN2, OUTPUT);          //Pin como salida para el driver motor del selector de herramienta
  pinMode(IN3, OUTPUT);          //Pin como salida para el driver motor del selector de herramienta
  pinMode(IN4, OUTPUT);          //Pin como salida para el driver motor del selector de herramienta
  pinMode(fan, OUTPUT);          //Pin como salida para el ventilador
  pinMode(LEDpanel, OUTPUT);     //Pin como salida para el panel LED
  pinMode(waterpump, OUTPUT);    //Pin como salida para la bomba de agua
  digitalWrite(waterpump, HIGH); //Inicialmente se desactiva la patilla del relé asociada a la bomba de agua

  servo1.attach(servoPin);           //Pin 13 para el servomotor
  setTime(15, 59, 30, 25, 01, 2020); //Se establece la hora actual para el sistema
  minutes = int(minute());           //Variable para hacer procesos cada X minutos
  hor = int(hour());                 //Variable hor para comparar las hora actual y hacer procesos cada hora
  daily = int(day());                //Variable para hacer procesos diarios
  dailyweeds = (int(day()) + 3);     //Variable para hacer procesos cada tres días

  dht.begin(); //Inicializa el sensor DHT 11

  Serial.begin(115200); //Puerto Serie del ESP32 (se ofrece información con el monitor serie)

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); //Serial2 para mandar instrucciones desde el ESP32 al Arduino UNO
  Serial.println("Serial Txd is on pin: " + String(TX));
  Serial.println("Serial Rxd is on pin: " + String(RX));

  //Se inicializa el Servidor Web
  //Se inicializa SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Se conecta al Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  //Se imprime la dirección IP Local del ESP32
  Serial.println(WiFi.localIP());

  //Ruta que lleva al usuario a la página inicial
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/frontpage.html"); //Al iniciar la plataforma web el sistema nos lleva a la página frontpage.html
  });

  server.serveStatic("/", SPIFFS, "/");

  //Botón Watering Plants activa proceso watering
  server.on(
  "/api/watering", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.print("You have pressed watering button");
    watering = true; //Variable que activará el proceso
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  //Botón Carry out Measurement activa proceso measurement
  server.on(
  "/api/measurement", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("You have pressed measurement button");
    measurementt = true; //Variable que activará el proceso
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  //Botón Remove Weeds activa proceso remove
  server.on(
  "/api/weeds", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.print("You have pressed remove weeds button");
    removeweeds = true; //Variable que activará el proceso
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  //Comandos de la página manual (adelante, atrás, izquierda, derecha, arriba, abajo, ventilador y panel LED)
  //Manual Commands (manual.html)(forward, backward, left, right, UP, DOWN, FAN, LEDpanel)
  //Permite al usuario mover la máquina mediante pequeños desplazamientos en cada eje
  //El usuario también puede encender o apagar el ventilador y/o el panel LED
  server.on(
  "/api/forward", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Going forward");
    Serial2.println("$X");
    delay(500);
    Serial2.println("G91  G21 G1 Y5 F360");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/backward", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Going backward");
    Serial2.println("$X");
    delay(500);
    Serial2.println("G91  G21 G1 Y-5 F360");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/right", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Going right");
    Serial2.println("$X");
    delay(500);
    Serial2.println("G91  G21 G1 X5 F350");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/left", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Gooing left");
    Serial2.println("$X");
    delay(500);
    Serial2.println("G91  G21 G1 X-5 F350");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/UP", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Going UP");
    Serial2.println("$X");
    delay(500);
    Serial2.println("G91  G21 G1 Z5 F300");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/down", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Going DOWN");
    Serial2.println("$X");
    delay(500);
    Serial2.println("G91  G21 G1 Z-5 F300");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/FAN", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("FAN switched ON");
    if (n1 == false)
    { //Debido al botón switch que se ha utilizado es necesario poner un contador (n1)
      digitalWrite(fan, HIGH);
      n1 = true;
    }
    else
    {
      digitalWrite(fan, LOW);
      n1 = false;
    }
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on(
  "/api/LED", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("LED panel switched ON");
    if (n2 == false)
    { //Debido al botón switch que se ha utilizado es necesario poner un contador (n2)
      digitalWrite(LEDpanel, HIGH);
      n2 = true;
    }
    else
    {
      digitalWrite(LEDpanel, LOW);
      n2 = false;
    }
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });
  //CIERRE Comandos de la página manual (adelante, atrás, izquierda, derecha, arriba, abajo, ventilador, panel LED)

  // Se mandan los datos de "measurement" Tª Y HUMEDAD Y HUMEDAD DEL SUELO
  server.on(
  "/api/datos", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("Sending data...");
    StaticJsonDocument<200> doc; //crea un documento Json (JavaScript Object Notation)
    doc["temperature"] = temperature;
    doc["air_humidity"] = airhumidity;
    doc["soilhumidity"] = soilhumidity;
    String output;
    serializeJson(doc, output); //escribe el documento JSON, con el valor de las lecturas
    request->send(200, "text/plain", output);
  });

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(404);
  });

  server.begin(); // Se inicializa el servidor
}

void loop()
{
  if (measurementt == true)
    measuremnt(); //Este proceso ofrece mediciones y si es necesario activa Fan o proceso watering
  if (removeweeds == true)
    removed(); //Proceso de eliminación de malas hierbas
  if (watering == true)
    water(); //Llama a la función para que se inicie el proceso de riego

  if (int(hour()) > hor) //Cada hora se medirá la luminosidad ambiente con LDR
  {
    activateLEDpanel(); //Se decidirá si activar el panel LED en función de la hora y el valor LDR
    hor = int(hour());
  }
  if (int(minute()) > 50)
    minutes = (int (minute())) - 50;   
  if (int(minute()) >= minutes + 10) //cada 10 minutos se medirá la temperatura y humedad y si son altas se activa ventilador
  {
    // Se hace medición de temperatura y humedad ambiente
    airhumidity = dht.readHumidity();    //Se lee la humedad relativa
    temperature = dht.readTemperature(); //Se lee la temperatura en grados centígrados (por defecto)
    //Se comprueba si ha habido algún error en la lectura
    if (isnan(airhumidity) || isnan(temperature))
    {
      Serial.println("Error getting data from sensor DHT11");
      return;
    }

    //Se imprime temperatura y humedad por puerto serie
    Serial.print("Humedad: ");
    Serial.print(airhumidity);
    Serial.print(" %\t");
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" *C ");
    if (temperature > 25 && airhumidity > 90)
    {
      digitalWrite(fan, HIGH); //Si la temperatura y humedad son altas se activa ventilador
      delay(100000);           //Se enciende el ventilador durante 10 min
      digitalWrite(fan, LOW);  //Se apaga el ventilador pasado el tiempo establecido
    }
    minutes = int (minute());
  }

  if (int(day()) > daily) //Cada día se hará una medición de Tª, humedad y humedad del suelo
  {
    measuremnt(); //Ofrece mediciones y, en función de estas, activa Fan o proceso watering
    daily = int(day());
  }

  if (int(day()) > dailyweeds) //Cada tres días se hace el proceso de eliminación de malas hierbas
  {
    removed();
    dailyweeds = (int(day()) + 3);
  }
}

//Proceso de riego (watering)
void water()
{
  Serial.println("I am going to water the plants");
  Serial2.println("$X"); //Desbloquea la máquina
  delay(500);
  Serial2.println("$H"); //Se hace Homing desde donde se iniciarán los movimientos
  delay(60000);          //60s
  Direction = true;
  int steps_left = 2055; 
  //Selección herramienta de riego (2055 pasos)
  while (steps_left > 0)
  {
    stepper();    //Avanza un paso
    steps_left--; //Resta un paso a la variable que cuenta los pasos que quedan por dar 
    delay(3);
  }
  delay(6000);

  Serial2.println("G21 G91");
  Serial2.println("G1 X60 F350");
  Serial2.println("G1 Z-90 F320");
  delay(32000);
  digitalWrite(waterpump, LOW);
  Serial2.println("G1 Y-190 F360");
  delay(33000);
  digitalWrite(waterpump, HIGH);
  Serial2.println("G1 Z80 F350");
  Serial2.println("G1 X120 F350");
  Serial2.println("G1 Z-90 F320");
  delay(52000);
  digitalWrite(waterpump, LOW);
  Serial2.println("G1 Y190 F360");
  delay(33000);
  digitalWrite(waterpump, HIGH);
  delay(6000);
  Serial2.println("$H");
  delay(60000);
  Direction = !Direction; //Se invierte la direccion de giro
  steps_left = 2055;
  while (steps_left > 0) //El cabezal de la máqina vuelve a su posición inicial
  {
    stepper();    //Avanza un paso
    steps_left--; //Resta un paso a la variable que cuenta los pasos que quedan por dar 
    delay(3);
  }

  watering = false;
}
//Fin proceso Watering

//Proceso eliminación malas hierbas (Remove weeeds)
void removed()
{
  Serial.println("I am going to remove weeds");
  Serial2.println("$X"); //Desbloquea la máquina
  delay(500);
  Serial2.println("$H"); //Se hace Homing desde donde se iniciarán los movimientos
  delay(60000);          //60s
  //movimiento hacia izquierda se selecciona la herramienta "quitar malas hierbas"
  int steps_left = 683;  
  Direction = true;
  while (steps_left > 0) //Selección de la herramienta de eliminación de malas hierbas
  {
    stepper();    // Avanza un paso
    steps_left--; //Resta un paso a la variable que cuenta los pasos que quedan por dar 
    delay(3);
  }
  delay(2000);
  Serial2.println("G21 G91");
  Serial2.println("G1 X80 F350");
  Serial2.println("G1 Y-170 F360");
  Serial2.println("G1 Z-110 F320");
  delay(63000);
  servo1.write(40); //Posición abajo en contacto con la tierra
  Serial2.println("G1 Y170 F360");
  delay(33000);
  servo1.write(130); //Posición arriba herramienta para eliminar malas hierbas
  Serial2.println("G1 Z80 F350");
  Serial2.println("G1 X130 F350");
  Serial2.println("G1 Y-170 F360");
  Serial2.println("G1 Z-80 F320");
  delay(90000);
  servo1.write(40); //Posición abajo en contacto con la tierra
  Serial2.println("G1 Y170 F360");
  delay(33000);
  servo1.write(130); //Posición arriba herramienta para eliminar malas hierbas (probar 140º)
  delay(6000);
  Serial2.println("$H"); //La CNC vuelve a su posición de Homing ($H) una vez ha eliminado las malas hierbas
  delay(60000);

  //El selector de herramientas vuelve a su posición inicial
  Direction = !Direction; //SE invierte la dirección de giro
  steps_left = 683;
  while (steps_left > 0) //Al revés la otra herramienta
  {
    stepper();    //Avanza un paso
    steps_left--; //Resta un paso a la variable que cuenta los pasos que quedan por dar
    delay(3);
  }
  delay(3000);

  removeweeds = false;
  Serial.println("I have finished removing weeds");
} //FIN proceso remove weeds

//Measurement
void measuremnt()
{
  Serial.println("I am going to measure");
  Serial2.println("$X"); //Desbloquea la máquina
  delay(500);
  Serial2.println("$H"); //Se hace Homing desde donde se iniciarán los movimientos
  delay(60000);          //60s

  //Se selecciona la herramienta de medida de humedad del suelo con el selector de herramientas
  Direction = false;
  while (total_steps > 0)
  {
    stepper();     //Avanza un paso
    total_steps--; //Resta un paso a la variable que cuenta los pasos que quedan por dar
    delay(3);
  }
  delay(3000);

  Serial2.println("G21 G91");
  Serial2.println("G1 X80 F350");
  Serial2.println("G1 Y-100 F360");
  Serial2.println("G1 Z-110 F320");
  delay(63000);
  // Hace medición con sensor de humedad suelo
  soilhumidity = analogRead(36);                     
  soilhumidity = map(soilhumidity, 0, 4095, 100, 0); //Convierte a valores de 0 a 100 siendo 0 muy seco
  Serial.print("soil humidity: ");
  Serial.println(soilhumidity);

  //Se hace medición de temperatura y humedad ambiente
  airhumidity = dht.readHumidity();    //Se lee la humedad relativa
  temperature = dht.readTemperature(); //Se lee la temperatura en grados centígrados (por defecto)
  //Se comprueba si ha habido algún error en la lectura
  if (isnan(airhumidity) || isnan(temperature))
  {
    Serial.println("Error getting data from sensor DHT11");
    return;
  }
  //Se imprime temperatura y humedad por puerto serie
  Serial.print("Humidity: ");
  Serial.print(airhumidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");
  //La CNC vuelve a su posición de Homing ($H) una vez ha realizado las mediciones
  Serial2.println("$H");
  delay(60000);

  //El selector de herramienta vuelve a su posición inicial 
  Direction = !Direction; // Se invierte la direceccion de giro
  total_steps = 683;      //Total de pasos que tiene que dar para volver a la posición inicial
  while (total_steps > 0) 
  {
    stepper();     //Avanza un paso
    total_steps--; //Resta un paso a la variable que cuenta los pasos que quedan por dar
    delay(3);
  }
  delay(5000);
  measurementt = false;

  if (soilhumidity < 20)
  {
    water(); //Si la humedad del suelo baja de un umbral se activa el proceso watering
  }
} //FIN proceso measurement

/*Funciones**/

//Funciones para el movimiento del motor selector de herramienta
void stepper() //Avanza un paso cada vez
{
  digitalWrite(IN1, Paso[Actual_Steps][0]);
  digitalWrite(IN2, Paso[Actual_Steps][1]);
  digitalWrite(IN3, Paso[Actual_Steps][2]);
  digitalWrite(IN4, Paso[Actual_Steps][3]);

  SetDirection();
}

void SetDirection() //Controla la dirección de giro del motor incrementando o decrementando el valor Actual_Steps
{
  if (Direction)
    Actual_Steps++;
  else
    Actual_Steps--;
  Actual_Steps = (Actual_Steps + 8) % 8; //ayuda a elegir el paso de la matriz
}

void activateLEDpanel() //Compara el valor de la LDR y la hora actual y activa panel LED
{
  LDRvalue = analogRead(LDR);  //Se lee el valor del LDR para determinar la luminosidad ambiente
  Serial.print("LDR value: "); //Valores de LDR 4095 - 0 (siendo 0 oscuridad)
  Serial.println(LDRvalue);
  Serial.print("It is: ");
  Serial.println(String(hour()) + ":" + String(minute()) + ":" + String(second()));
  if ((LDRvalue < 410) && (istimetoactivateLED()))
  {
    Serial.print("The ambient light level is NOT suficient. \nThe LED panel must be switched ON\n");
    digitalWrite(LEDpanel, HIGH);
  }
  else
  {
    digitalWrite(LEDpanel, LOW);
    Serial.println("It is not Time or it is not necesary to switch LED panel ON\n");
  }
  delay(4000); //no vale
}

boolean istimetoactivateLED() //Determina si la hora actual es horario diurno
{
  int inicialTime = (inicialhour * 60) + inicialmin;
  int finalTime = (finalhour * 60) + finalmin;
  int actualTime = (hour() * 60) + minute();
  Serial.println(inicialTime);
  Serial.println(finalTime);
  Serial.println(actualTime);
  if ((inicialTime <= actualTime) && (actualTime < finalTime)) //Compara si el horario en el que se encuentra la máquina
  {                                                            //Comprueba si el sistema está dentro del horario diurno
    return true;                                               
  }
  else
    return false;
}
