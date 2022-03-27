//SE CARGAN LIBRERIAS------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <FS.h>
#endif
#include <ESPAsyncWebServer.h>
#include <time.h>

//DECLARACION DE VARIABLES---------------------------------------------------------------------------------------------------------------
//VARIABLES MOTOR UWU
const int dirPin = 27;
const int stepPin = 14; 
const int steps = 200;
int stepDelay;
//SE CREA EL PUERTO PARA EL WEBSERVER(PUERTO 80)
AsyncWebServer server(80);
// SE CARGAN LAS CREDENCIALES WIFI
const char* ssid   = "Fibertel WiFi320 2.4GHz";
const char* password = "MARISAM2020";
// SE DECLARAN VARIABLES PARA EL HTTP WEBSERVER Y LA VARIABLE DE ESTADO DEL MOTOR
const char* PARAM_INT1 = "inputInt1";
const char* PARAM_INT2= "inputInt2";
const char* PARAM_INT3 = "inputInt3";
boolean A=false;
//SERVIDOR PARA SACAR EL TIEMPO REAL
const char* ntpServer = "pool.ntp.org";//PAGINA O SERVIDOR NTP
const long  gmtOffset_sec = -10800;//Desplazamiento GMT
const int   daylightOffset_sec = 3600;//Desplazamiento de luz diurna


//FUNCIONES---------------------------------------------------------------------------------------------------------------------------------

//PAGINA NO ENCONTRADA ERROR404
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
//FUNCIONES COMPLEMENTARISA PARA SPPIFS---------------------------
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

// REEMPLAZADOR DE "VALORES ACTUALES" DE LA PAGINA WEB----------------------
String processor(const String& var){
  //Serial.println(var);
  if(var == "inputInt1"){
    return readFile(SPIFFS, "/inputInt1.txt");
  }
  else if(var == "inputInt2"){
    return readFile(SPIFFS, "/inputInt2.txt");
  }
  else if(var == "inputInt3"){
    return readFile(SPIFFS, "/inputInt3.txt");
  }
  return String();
}
//FUNCION MOTOR----------------------------------------------------------------
void encenderMotor(int tiempoGiro){
     //GIRO DEL MOTOR
   Serial.print("Tiempo que el motor girara: ");
   Serial.print(stepDelay);
   digitalWrite(dirPin, LOW);
   stepDelay =2200;
   for(int i=0;i<tiempoGiro;i++){
   for (int x = 0; x < 260; x++) {
      digitalWrite(stepPin, HIGH);
   delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepDelay);
   }
   
   }
   Serial.println("se termino de dar la ración");
   delay(5000);
   }

//SETUP Y LOOP-------------------------------------------------------------------------------------------------------------------------------
//SETUP----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  //MOTOR UWU--------------------------------
    // Marcar los pines como salida
   pinMode(dirPin, OUTPUT);
   pinMode(stepPin, OUTPUT);
   
  //INICIA SPPIFS----------------------------
  
  #ifdef ESP32
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #else
    if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #endif

  //WIFI-------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //SERVIDOR NTP------------------------------

    // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  //REQUEST HTML(GET)----------------------

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  //ACCION BOTON------------------------------------------------------------------------------------------------------------------------------(SE ACTIVA EL MECANISMO)
 server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
     int tiempoGiro = readFile(SPIFFS, "/inputInt1.txt").toInt();
     encenderMotor(tiempoGiro);
  });
  // Route to load style.css fil e
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_INT1)) {
      inputMessage = request->getParam(PARAM_INT1)->value();
      writeFile(SPIFFS, "/inputInt1.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_INT2)) {
      inputMessage = request->getParam(PARAM_INT2)->value();
      writeFile(SPIFFS, "/inputInt2.txt", inputMessage.c_str());
    }
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else if (request->hasParam(PARAM_INT3)) {
      inputMessage = request->getParam(PARAM_INT3)->value();
      writeFile(SPIFFS, "/inputInt3.txt", inputMessage.c_str());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();

    
}
//LOOP------------------------------------------------------------------------------------------------
void loop() {
  // ESCRITURA CONSTANTE DE ARCHIVOS SPPIFS------------------------
  int tiempoGiro = readFile(SPIFFS, "/inputInt1.txt").toInt();
  Serial.print("tiempoGiro: ");
  Serial.println(tiempoGiro);
   
  int horaAlarma = readFile(SPIFFS, "/inputInt2.txt").toInt();
  Serial.print("*** Your inputInt2: ");
  Serial.println(horaAlarma);
  
  int minutoAlarma = readFile(SPIFFS, "/inputInt3.txt").toInt();
  Serial.print("*** Your inputInt3: ");
  Serial.println(minutoAlarma);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if(horaAlarma>24){
    horaAlarma=0;
  }
  if(horaMinuto>60){
    horaMinuto=0;
  }

  //ACTUALIZACION VARIABLES DE TIEMPO--------------------------------
//TIEMPO DE UN SERVIDOR NTP------------------------------------------
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
    char timeMinute[3];
  strftime(timeMinute,3, "%M", &timeinfo);
  Serial.println(timeMinute);

  int horaActual;
   horaActual = atol(timeHour);
   Serial.println(horaActual);
     int minutoActual;
   minutoActual = atol(timeMinute);
   Serial.println(minutoActual);

  
//ALARMA DE PETFEEDER----------------------------------------------------------------------------------------------------------------------(SE ACTIVA EL MECANISMO)
if(horaAlarma==horaActual&&minutoAlarma==minutoActual){
  encenderMotor(tiempoGiro);
}
  delay(5000);

 
}
