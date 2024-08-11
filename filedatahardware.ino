#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include<MQUnifiedsensor.h>
#include<MQ135.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Apni Lawao"
#define WIFI_PASSWORD "ASAD@123478"

// Insert Firebase project API Key
#define API_KEY "AIzaSyC7HJ2Wwrn7hfPlVAg89T-Yn9q9FBCVCWY"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://emissiguard-601b5-default-rtdb.firebaseio.com/" 
const int sensormq7= 34; 
const int sensormq2= 33;
const int sensormq135= 32; 
float mapfloat(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}
//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
float tvalue=100.0;
bool signupOK = false;

void setup(){
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
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
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
  int sensorvalmq135 = analogRead(sensormq135);
  float volt135 =sensorvalmq135*(5.0/1023.0);
  float ppmmq135 = mapfloat(volt135, 0.0,5.0,0.0,100.0);
  Serial.print("mq135 value  ");
  Serial.print(ppmmq135);
  Serial.print("ppm");
   if (Firebase.RTDB.setFloat(&fbdo, "user_data/-NfygVZ0VuFP0c8Bu-P_/mq135",ppmmq135)){
      Serial.println("     send");
    }
    else {
      Serial.println("    FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  int sensorvalmq2 = analogRead(sensormq2);
  float volt2 =sensorvalmq2*(5.0/1023.0);
  float ppmmq2 = mapfloat(volt2, 0.0,5.0,0.0,100.0);
   Serial.print("mq2 value   ");
  Serial.print(ppmmq2);
  Serial.print("ppm");
  
    if (Firebase.RTDB.setFloat(&fbdo, "user_data/-NfygVZ0VuFP0c8Bu-P_/mq2", ppmmq2)){
      Serial.println("    send");
    }
    else {
      Serial.println("    FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  int sensorvalmq7 = analogRead(sensormq7);
  float volt7 =sensorvalmq7*(5.0/1023.0);
  float ppmmq7 = mapfloat(volt7, 0.0,5.0,0.0,100.0);
  Serial.print("mq7 value   ");
  Serial.print(ppmmq7);
  Serial.print("ppm");
    if (Firebase.RTDB.setFloat(&fbdo, "user_data/-NfygVZ0VuFP0c8Bu-P_/mq7",ppmmq7)){
      Serial.println("   send");
    }
    else {
      Serial.println("   FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if(count< 4){
    if(ppmmq2> tvalue ||ppmmq7> tvalue || ppmmq135> tvalue){
      count++;
    }}
    else{

    }
    Serial.print(count);
      if (Firebase.RTDB.setInt(&fbdo, "user_data/-NfygVZ0VuFP0c8Bu-P_/notificationCount",count)){
      Serial.println("   send");
    }
    else {
      Serial.println("   FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }}
    
  delay(30000);
    
}