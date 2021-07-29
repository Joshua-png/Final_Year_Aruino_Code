#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D1,D2); //RX TX

//Provide the token generation process info.
#include "addons/TokenHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "DESKTOP-PVGRMU3 8897"
#define WIFI_PASSWORD "01234567"

/* 2. Define the API Key */
#define API_KEY "AIzaSyACUuNXJpyyyIdwYbpB091uyeafkFFAicY"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "userapplication-4369d"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "admin@gmail.com"
#define USER_PASSWORD "admin123"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

unsigned long dataMillis = 0;
String receivedId;
float sentAmount;
//float amountNode;
int amountNode;
//String documentPath;
String content;
int counter = 0;
int realAmount;

int LED = 2;
int thirteen = 2;
int one = 1;
int cont = 1;
void setup()
{

   Serial.begin(9600);
   s.begin(9600);

    //s.print(thirteen);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        s.print(thirteen);
        digitalWrite(LED,LOW);
        Serial.print(".");
        delay(300);
    }
    s.println(one);
    pinMode(13,OUTPUT);
    digitalWrite(13,HIGH);
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&config, &auth);
    
    Firebase.reconnectWiFi(true);
}

void loop(){
   // Serial.println(WiFi.status());
    while(WiFi.status() != WL_CONNECTED){
      
       digitalWrite(13,LOW);
       WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

      
       while(WiFi.status() != WL_CONNECTED){
        s.println(thirteen);
        Serial.print(".");
        delay(300);
      }

       if(WiFi.status() == WL_CONNECTED){
          
          Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

          /* Assign the api key (required) */
          config.api_key = API_KEY;
      
          /* Assign the user sign in credentials */
          auth.user.email = USER_EMAIL;
          auth.user.password = USER_PASSWORD;
      
          /* Assign the callback function for the long running token generation task */
          config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
      
          Firebase.begin(&config, &auth);
          
          Firebase.reconnectWiFi(true);

          digitalWrite(13,HIGH);
       }
    }
    
     
   if (Firebase.ready() && WiFi.status() == WL_CONNECTED){
   s.println(one);
//    s.end();
//    delay(10000);
    //delay(3000);
//    s.begin(9600);
    while(s.available() > 0){
        receivedId = s.readString();
        Serial.println("Received from NFC card: "+receivedId);
        receivedId.trim();

        String documentPath = "users/"+receivedId;
        String mask = "Account_Balance";

    

        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str())){
            //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
              FirebaseJson json;
              FirebaseJsonData jsonData;
              
            
              json.setJsonData(fbdo.payload());
            
              //json.get(jsonData, "fields/Singapore/mapValue/fields/population/integerValue");
              json.get(jsonData, "fields/Account_Balance/integerValue");
              //Serial.println(jsonData.stringValue);

              String amount = jsonData.stringValue;
              //Serial.println("String value: ");
              //Serial.println(amount);
              //float realAmount = amount.toFloat();
              realAmount = amount.toInt();
              //Serial.println("Float value: ");
              //Serial.println(realAmount);
              realAmount -= 1;
              s.print(realAmount);
              Serial.println(realAmount);
              
             }
             


        String content;
        FirebaseJson js;

        //String documentPath = "users/"+receivedId;
        //amountNode = s.parseInt();
  
        js.set("fields/Account_Balance/integerValue", String(realAmount).c_str());
        js.toString(content);

        Serial.print("Amount to be written: ");
        Serial.println(realAmount);

        /** if updateMask contains the field name that exists in the remote document and 
         * this field name does not exist in the document (content), that field will be delete from remote document
        */

        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.c_str(), "Account_Balance" /* updateMask */))
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        else
            Serial.println(fbdo.errorReason());
        break;
        }
      //cont = 0;  
      //break;            
     }else if(WiFi.status() != WL_CONNECTED){
         s.println(thirteen);
         Serial.println(fbdo.errorReason());
      }
    
  
  }
 
