#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <SoftwareSerial.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);  // The Adafruit PN532 Shield
SoftwareSerial s(10,11);

String cardID , amount, Id;
float a;
String am;
boolean finished;

//just added
//float amountNode;
int amountNode;
int buzzer = 8;
int wifiCode;


void setup(void) {
  Serial.begin(9600);
  s.begin(9600);
  Serial.println("NFC TAG READER");
  nfc.begin();
  Serial.println("\nScan your NFC tag on the PN532 NFC!");
  
}

void loop(void) {
  Serial.println(wifiCode);
//  while(s.available());
//  {
//       s.read();   
//  }
   
//     while(s.available() == 0){
//           s.setTimeout(100);
//    }
Serial.println(wifiCode);
  while(s.available() > 0){
      wifiCode = s.parseInt();
      Serial.println(wifiCode);
      //while(s.available());
      //Serial.println(wifiCode);
      break;
  }

 s.flush();
  
Serial.println(wifiCode);

  if(wifiCode == 1){
        Serial.println(wifiCode);
        Serial.println("connected");
        if (nfc.tagPresent()){
            Serial.println("NFC tag Found!\n");
            NfcTag tag = nfc.read();
        
            //Display Card ID
            //Serial.print("UID: ");
            cardID = tag.getUidString();
            Serial.println(cardID);
        
            if (tag.hasNdefMessage())
            {
              
              NdefMessage message = tag.getNdefMessage();
              NdefRecord record[2];
              String payloadAsString[2];
              //amount, Id;
              
              
              // Cycling through the records
              int recordCount = message.getRecordCount();
              for (int i = 0; i < recordCount; i++)
              {
        
                //reading the records containing the payload needed
                record[i] = message.getRecord(i);
        
                int payloadLength = record[i].getPayloadLength();
                byte payload[payloadLength];
                record[i].getPayload(payload);
        
        
                payloadAsString[i] = ""; // Processes the message as a string
                for (int c = 3; c < payloadLength; c++) 
                {
                  payloadAsString[i] += (char)payload[c];
                }
                
              }
        
              for(int i=0; i < recordCount;i++ ){
               
                     //Amount payload as a String
                     amount = payloadAsString[0];
        
                     //Amount in Floatand converting to String
                     //a = amount.toFloat() - 1;
                     //am = String(a,2); 
        
                     Id =  payloadAsString[1];
              }
        
                     s.println(Id+'\n');
                     
                     while(s.available()){
                       s.read();
                     }
                    
                     while(s.available() == 0){
                       s.setTimeout(100);
                     }
                     
                     while(s.available() > 0){
                      amountNode = s.parseInt();
                      break;
                     }
                     while(s.available());
                     String samountNode = String(amountNode);
                     NdefMessage messageA = NdefMessage();
        //
                      //Writing the amount to the NFC card
                       messageA.addTextRecord(samountNode);
                       //Writing the Id to the NFC card
                       messageA.addTextRecord(Id); 

                        
                      bool success = nfc.write(messageA);
                        nfc.write(messageA);
                       if (success) {
                          tone(buzzer, 1000);   // Send 1KHz sound signal...
                          delay(700);              // ...for 1 sec
                          noTone(buzzer);       // Stop sound...
                          delay(100); 
                          tone(buzzer, 1000);   // Send 1KHz sound signal...
                          delay(700);              // ...for 1 sec
                          noTone(buzzer);       // Stop sound...
                          delay(100);  
                       } else {
                         Serial.println("Write failed.");
                       }
                    
                        Serial.print("ID From Arduino to Nodemcu: ");
                        Serial.println(Id);
                        Serial.print("Firebase amount received : ");
                        Serial.println(amountNode);
                
                        
                        delay(3000);
            }
          }
//      break  ;
       
  }else if(wifiCode != 1){
    Serial.println("Not connected");
    delay(1000);
  }

  
 

}
