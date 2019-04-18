//Transmitter (Arduino Mega)
#include <XBee.h>
XBee xbee = XBee();

#include <SPI.h>
#include <MFRC522.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(8, 2, 4, 5, 6, 7);
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Capstone :");
//  delay(100);/

  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
void loop() {

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
   mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
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
  if (content.substring(1) == "15 FD D5 D1") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();  // Clearing the LCD module
    lcd.setCursor(0,0);  // Set the LCD curser at the initial location
    lcd.print("Authorized");
    lcd.setCursor(0,1);
    Serial.println("Authorized access");
    Serial.println();
    uint8_t data[] = {'A'};
  XBeeAddress64 addr64 = XBeeAddress64();
  addr64.setMsb(0x0013A200);//XXXXX -> Msb address of router/end node
  addr64.setLsb(0x40B96DBC);//XXXXX -> Lsb address of router/end node 
  ZBTxRequest zbTx = ZBTxRequest(addr64, data, sizeof(data));
  xbee.send(zbTx);
delay(100);
    delay(300);
  }
 
 else   {
    lcd.clear();  // Clearing the LCD module
    lcd.setCursor(0,0);  // Set the LCD curser at the initial location
    lcd.print("Denied");
    lcd.setCursor(0,1);
    Serial.println(" Access denied");
    uint8_t data[] = {'D'};
  XBeeAddress64 addr64 = XBeeAddress64();
  addr64.setMsb(0x0013A200);//XXXXX -> Msb address of router/end node
  addr64.setLsb(0x40B96DBC);//XXXXX -> Lsb address of router/end node 
  ZBTxRequest zbTx = ZBTxRequest(addr64, data, sizeof(data));
  xbee.send(zbTx);
delay(100);
    delay(300);
  }
  
  
}
