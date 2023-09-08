#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
  #include <SD.h>
#elif defined(ESP8266)
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Insert Firebase project API Key
#define API_KEY "AIzaSyD"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://invernadero" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
float floatValue;
bool signupOK = false;
int distancia, distancia_tmp;

int getDistancia(){
  int distancia;
  Firebase.RTDB.getInt(&fbdo, "Distancia");
  distancia = fbdo.intData();
  return distancia;
}


void setup(){
  Serial.begin(9600); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  distancia = getDistancia();
  if(distancia != distancia_tmp){
    distancia_tmp=distancia;
    Serial.print(distancia);
  }
  delay(200);
}
