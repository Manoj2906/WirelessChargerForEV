#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27,16,2);
 #define BLYNK_PRINT Serial
#define SS_PIN 21
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 int b1= 600;
 int b2 = 1200;
 int b3= 1800; 
 int count = 0;
 Servo myServo;
 int pos = 0;
#define BLYNK_TEMPLATE_ID "TMPL3Yen2RjGO"
#define BLYNK_TEMPLATE_NAME "dsce"
//#define BLYNK_AUTH_TOKEN "bCVGSiH0Nxwloy9PDHWFA_W3nKWX05_Y"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
float voltage;

int bat_percentage;

int analogInPin  = 35; // Analog input pin

int sensorValue;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "bCVGSiH0Nxwloy9PDHWFA_W3nKWX05_Y";
float calibration = 0.36;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Manoj";
char pass[] = "Manoj0106";


void setup() 
{
  
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(4, OUTPUT); //BUZZER
  pinMode(27,OUTPUT); //relay
  pinMode(analogInPin,INPUT);
   myServo.attach(32);
  myServo.write(0);
  Blynk.begin(auth, ssid, pass);
}
void loop() 
{
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "51 36 C1 1B") //change here the UID of the card/cards that you want to give access  20 mins
  { 
    count = count +1;
     Blynk.run();
     Blynk.virtualWrite(V0,count);
    b1=b1-10;
   Serial.print(b1);
    Blynk.run();
     Blynk.virtualWrite(V1,b1);
   
    digitalWrite(4,HIGH);
    delay(100);
    digitalWrite(4,LOW);
     digitalWrite(27,HIGH);
     myServo.write(90);
     delay(10000);
     digitalWrite(27,LOW);
    myServo.write(pos);
      digitalWrite(4,HIGH);
      delay(200);
      digitalWrite(4,LOW);
    // digitalWrite(4,HIGH);
    /* if(b1==0){
     digitalWrite(27,LOW);
         tone(20000,4,1000);
    digitalWrite(4,HIGH);
    myServo.write(pos);
      }*/
     
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
  //--------------------------------------------------------------------------------------------------NEW CARD -------------------------------------------------------------------------------------------
  if (content.substring(1) == "D3 AC 40 13") //change here the UID of the card/cards that you want to give access  40 mins
  { 
      count = count +1;
     Blynk.run();
     Blynk.virtualWrite(V0,count);
     b2=b2-15;
    Blynk.run();
     Blynk.virtualWrite(V2,b2);
    
     digitalWrite(4,HIGH);
    delay(100);
    digitalWrite(4,LOW);
     digitalWrite(27,HIGH);
     myServo.write(90);
     delay(10000);
     digitalWrite(27,LOW);
     myServo.write(pos);
      digitalWrite(4,HIGH);
      delay(200);
      digitalWrite(4,LOW);
      /*if(b2=0){
      
         digitalWrite(27,LOW);
    tone(20000,4,1000);
    digitalWrite(4,HIGH);
    
    myServo.write(pos);
      }*/
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
  //--------------------------------------------------new---------------------------------------------------------
   if (content.substring(1) == "43 E0 97 2F") //change here the UID of the card/cards that you want to give access  60 mins
  {
      count = count +1;
     Blynk.run();
     Blynk.virtualWrite(V0,count);
     b3=b3-20;
    Blynk.run();
     Blynk.virtualWrite(V3,b3);
  
   
    digitalWrite(4,HIGH);
    delay(100);
    digitalWrite(4,LOW);
     digitalWrite(27,HIGH);
     myServo.write(90);
     delay(8000);
     digitalWrite(27,LOW);
     myServo.write(pos);
      digitalWrite(4,HIGH);
      delay(200);
      digitalWrite(4,LOW);
        /*if(b3=0){
      
         digitalWrite(27,LOW);
         tone(20000,4,1000);
    digitalWrite(4,HIGH);
    myServo.write(pos);
      }*/
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
  //--------------------------------------new---------------------------------------------
 else   {
 
    digitalWrite(27,LOW);
    digitalWrite(4,HIGH);
    delay(100);
    digitalWrite(4,LOW);
    delay(100);
    myServo.write(pos);
    Serial.println(" Access denied");
    delay(3000);
  }
  sensorValue = analogRead(analogInPin);

//multiply by two as voltage divider network is 100K & 100K Resistor

voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); 

//2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage

bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100); 

if (bat_percentage>= 100)

{

bat_percentage = 100;

}

if (bat_percentage<= 0)

{

bat_percentage = 1;

}

//send data to blynk
Blynk.run();

Blynk.virtualWrite(V5, voltage);  // for battery voltage

Blynk.virtualWrite(V4, bat_percentage);  // for battery percentage
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)

{

return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}
 
