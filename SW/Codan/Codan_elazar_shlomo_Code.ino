#include <SPI.h>
#include "PN532_SPI.h"
#include "PN532.h"
#include "NfcAdapter.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>


LiquidCrystal lcd(8,9,4,5,6,7);

PN532_SPI interface(SPI, 53);
int pin1=24, pin2=23, pin3=22, pin4=28, pin5=27, pin6=26, pin7=25, pinLED=13, pinESP=44;
String input="";
String password ;
String password_master;
NfcAdapter nfc = NfcAdapter(interface);
String knownID[] = {"7A D6 40 69", "D4 B6 BC 99", "04 D6 BC 99", "94 B7 BC 99", "54 F9 BC 99", "94 17 BC 99", "54 D6 BC 99", "04 F0 BB 99", "24 D3 BB 99", "94 E4 BC 99", "93 9F 5F D1",
                    "EA D3 A8 6A", "5A 32 B3 6A", "AA 20 B1 6A", "0A BC 44 69", "8A 5B AF 6A", "5A 29 4C 69", "FA F1 B9 6A", "5A FD AE 6A", "BA F8 3C 69", "DA 6D BA 6A"};
String names[] = {"Or", "Omry", "Yogev", "Israel", "Netanel", "Adar", "Vladi", "Reuven", "Shiya", "Elazar", "Amit", "Mark", "Adar", "Michael", "Theresa", "Sims", "Sims",
                  "Algos", "Algos", "Kohelet", "Ronny"};
String NFCPassword = "Message encoded.";
int lastNum, input_length;
bool lastButton1, lastButton2, lastButton3, lastButton4, lastButton5, lastButton6, lastButton7, lastButton8, lastButton9, lastButton0, lastButtonS, lastButtonH;
bool change_pass = false;

bool check = 1;

void readColumns();
void checkNFC();
void openDoor();
void openDoor(String nameIn);
void clearScreen();
void readColumns2(int index);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin5, INPUT_PULLUP);
  pinMode(pin6, INPUT_PULLUP);
  pinMode(pin7, INPUT_PULLUP);
  pinMode(pinESP,INPUT_PULLUP);
  pinMode(3, OUTPUT);
  digitalWrite(3,0);
  
  nfc.begin();
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);

  lcd.print("Enter Password:");

//    int num2 = xxxx;
//    int num1 =xxxx;
//    EEPROM.put(0,num1/256);
//    EEPROM.put(1,num1%256);
//
//    EEPROM.put(3,num2/256);
//    EEPROM.put(4,num2%256);
    int temp = EEPROM.read(0);
    int temp1 = EEPROM.read(1);
    int temp2 =EEPROM.read(3);
    int temp3 = EEPROM.read(4);

    password =String(temp*256+temp1);
    password_master = String(temp2*256+temp3);
   
}
void loop() {
  readColumns();
  if(input.length()==0){
    checkNFC();
    if(!digitalRead(pinESP)){
      openDoor("Guest");
    }
  }
  lcd.setCursor(0,1);
  lcd.print(input);
  Serial.println(input);
}
void checkNFC2(){
  if (nfc.tagPresent()){
    NfcTag tag = nfc.read(); // read the NFC tag
    String presentID = tag.getUidString();
    String presentMessege;
    
    if (tag.hasNdefMessage()){
      NdefMessage message = tag.getNdefMessage();
      
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++){
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        String payloadAsString = "";
        for (int c = 3; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        presentMessege = payloadAsString;
      }
    }
    if(presentMessege==NFCPassword){
        if (presentID==knownID[9] ||presentID==knownID[19]){
              clearScreen();
              lcd.setCursor(0,0);
              lcd.print("Enter new pass :");
              while(change_pass == false)
              {
                lcd.setCursor(0,1);
                lcd.print(input);
                readColumns2(2);
              }
              input = "";
              lastNum = 0;
        }
        else{
      clearScreen();
      lcd.setCursor(0,0);
      lcd.print("Wrong Tag");
      delay(1500);
      change_pass = true;
      setup();
    }
      }
    }
  }
void checkNFC(){
  if (nfc.tagPresent()){
    NfcTag tag = nfc.read(); // read the NFC tag
    Serial.println(tag.getUidString());
    String presentID = tag.getUidString();
    String presentMessege;
    
    if (tag.hasNdefMessage()){
      NdefMessage message = tag.getNdefMessage();
      
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++){
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        String payloadAsString = "";
        for (int c = 3; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        presentMessege = payloadAsString;
      }
    }
    Serial.println(presentMessege);
    if(presentMessege==NFCPassword){
      for(int i=0; i<sizeof(knownID);i++){
        if (presentID==knownID[i]){
          openDoor(names[i]);
        }
      }
    }else{
      clearScreen();
      lcd.setCursor(0,0);
      lcd.print("Wrong Tag");
      delay(1500);
      setup();
    }
  }
}
void readColumns(){
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin1, LOW);
  if(!digitalRead(pin4) && !lastButton1){
    addNum("1");
  }else if(!digitalRead(pin5)&& !lastButton4){
    addNum("4");
  }else if(!digitalRead(pin6)&& !lastButton7){
    addNum("7");
  }else if(!digitalRead(pin7)&& !lastButtonS){
    deleteLetter();
  }
  lastButton1 = !digitalRead(pin4);
  lastButton4 = !digitalRead(pin5);
  lastButton7 = !digitalRead(pin6);
  lastButtonS = !digitalRead(pin7);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  if(!digitalRead(pin4)&& !lastButton2){
    addNum("2");
  }else if(!digitalRead(pin5)&& !lastButton5){
    addNum("5");
  }else if(!digitalRead(pin6)&& !lastButton8){
    addNum("8");
  }else if(!digitalRead(pin7)&& !lastButton0){
    addNum("0");
  }
  lastButton2 = !digitalRead(pin4);
  lastButton5 = !digitalRead(pin5);
  lastButton8 = !digitalRead(pin6);
  lastButton0 = !digitalRead(pin7);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  if(!digitalRead(pin4)&& !lastButton3){
    addNum("3");
  }else if(!digitalRead(pin5)&& !lastButton6){
    addNum("6");
  }else if(!digitalRead(pin6)&& !lastButton9){
    addNum("9");
  }else if(!digitalRead(pin7)&& !lastButtonH){
    checkPassword();
  }
  lastButton3 = !digitalRead(pin4);
  lastButton6 = !digitalRead(pin5);
  lastButton9 = !digitalRead(pin6);
  lastButtonH = !digitalRead(pin7);
}
void readColumns2(int index){
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin1, LOW);
  if(!digitalRead(pin4) && !lastButton1){
    addNum("1");
  }else if(!digitalRead(pin5)&& !lastButton4){
    addNum("4");
  }else if(!digitalRead(pin6)&& !lastButton7){
    addNum("7");
  }else if(!digitalRead(pin7)&& !lastButtonS){
    deleteLetter();
  }
  lastButton1 = !digitalRead(pin4);
  lastButton4 = !digitalRead(pin5);
  lastButton7 = !digitalRead(pin6);
  lastButtonS = !digitalRead(pin7);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  if(!digitalRead(pin4)&& !lastButton2){
    addNum("2");
  }else if(!digitalRead(pin5)&& !lastButton5){
    addNum("5");
  }else if(!digitalRead(pin6)&& !lastButton8){
    addNum("8");
  }else if(!digitalRead(pin7)&& !lastButton0){
    addNum("0");
  }
  lastButton2 = !digitalRead(pin4);
  lastButton5 = !digitalRead(pin5);
  lastButton8 = !digitalRead(pin6);
  lastButton0 = !digitalRead(pin7);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  if(!digitalRead(pin4)&& !lastButton3){
    addNum("3");
  }else if(!digitalRead(pin5)&& !lastButton6){
    addNum("6");
  }else if(!digitalRead(pin6)&& !lastButton9){
    addNum("9");
  }else if(!digitalRead(pin7)&& !lastButtonH){
    if(input.toInt()<10000 && input.toInt() > 999)
    {
       change_pass = true;
       if(index == 1)
       {
          change_password(input.toInt());
       }
       else
       {
          change_password_master(input.toInt());
       }
       clearScreen();
       lcd.setCursor(0,0);
       lcd.print("Password changed!");
       input = "";
       delay(1500);
    }
    else
    {
       input = "";
       lcd.setCursor(0,1);
       lcd.print("password invalid");
       delay(1500);
       clearRow(1);
       
    }
  }
  lastButton3 = !digitalRead(pin4);
  lastButton6 = !digitalRead(pin5);
  lastButton9 = !digitalRead(pin6);
  lastButtonH = !digitalRead(pin7);
}

void addNum(String num){
  /*used to add a new entered digit to the input variable*/
  lastNum=num.toInt();
  input += num;
  input_length = input.length();
}

void deleteLetter(){
  String newInput;
  if(input.length()>1){
    for(int i=0; i<input.length()-1; i++){
      newInput += input.charAt(i);
    }
  }else if(input.length()==1){
    newInput = "";
  }
  clearRow(1);
  input=newInput;
}

void clearRow(int row){
  /*used to clear a specified row*/
  for(int i=0; i<16; i++){
    lcd.setCursor(i,row);
    lcd.print(" ");
  }
}
void clearScreen(){
  /*a function to clear the screen from any input the user entered*/
  clearRow(0);
  clearRow(1);
}
void checkPassword(){
  /*the manage of the admin passcode.. if the password is correct you are given options to 1) change the password or 2)to open the door(with the name of "manager").*/
  if(input.equals(password_master)){
    clearScreen();
    lcd.setCursor(0,0);
    lcd.print("1:To change pass");
    lcd.setCursor(0,1);
    lcd.print("2:To open Door");
    
    check = true;
    while(check)
    {
        readColumns();
        
       if(lastNum == 1)
       {
          change_pass = false;
          input = "";
          lastNum = 0;
          check = false;
          clearScreen();
          lcd.setCursor(0,0);
          lcd.print("Enter new pass :");
          while(change_pass == false)
          {
            readColumns2(1);
            lcd.setCursor(0,1);
            lcd.print(input);
          }
          setup();  
       }
       else if(lastNum == 2)
       {
          lastNum = 0;   
          check = false;  
          openDoor("Manager!");            
       }
       else if(lastNum == 3)
       {
          change_pass = false;
          input = "";
          lastNum = 0;
          check = false;
          clearScreen();
          lcd.setCursor(0,0);
          lcd.print("Insert your ");
          lcd.setCursor(0,1);
          lcd.print("Tag, MASTER! ");
          while(change_pass == false)
          {
            checkNFC2();
          }
          setup();    
       }
    }
  }
  else
  {
    lcd.setCursor(0,1);
    if(input.equals(password))
  {
    openDoor("Guest");
  }
    else
    {
      lcd.print("Wrong Password");
      delay(1500);
      input="";
      setup();
    }
  }  
}

void openDoor(String nameIn){
  /*opens the door with a name if given one.. alternative is to open to guest*/
  clearScreen();
  lcd.setCursor(0,0);
  lcd.print("Welcome "+nameIn);
  lcd.setCursor(0,1);
  lcd.print("Come In");
  digitalWrite(3,1);
  delay(4000);
  input="";
  setup();
}
void change_password(int pass)
{
    /*writes to memory the new password entered*/
    EEPROM.put(0,pass/256);
    EEPROM.put(1,pass%256);
    password = String(pass); 
}
void change_password_master(int pass)
{
    /*writes to memory the new password entered*/
    EEPROM.put(3,pass/256);
    EEPROM.put(4,pass%256);
    password_master = String(pass); 
}
