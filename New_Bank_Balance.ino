#include <Password.h> //http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

Password password = Password("9999");

const byte ROWS = 4; // Four rows
const byte COLS = 3; //  columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { 5,4,3,2 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 8,7,6 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16,2);


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  lcd.begin();
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print("Scan card...");
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
}
// --------------------------------------------------------------------
void loop() {
  readsuccess = getid();
 
  if(readsuccess){
    
    if (StrUID=="A20DF533") {
      password = Password( "5432" );
      ScanPIN();
    }
    else if(StrUID == "291CCF59"){
      password = Password( "1234" );
      ScanPIN();
    }
    
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Status : Unknown");
    }
  }

  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scan card...");  
}
// --------------------------------------------------------------------
int getid(){  
  if(!mfrc522.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    return 0;
  }
 
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}
// --------------------------------------------------------------------
void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

// ----------------------------------------------------------------------
//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
  Serial.print("Pressed: ");
  Serial.println(eKey);
  switch (eKey){
    case '*': checkPassword(); break;
    case '#': ScanPIN(); break;
    default: password.append(eKey);
     }
  }
}

void checkPassword(){
  if (password.evaluate()){
    Serial.println("Success...");

    if (StrUID=="A20DF533") {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Name: Ezzel");
      lcd.setCursor(0,1);
      lcd.print("Balance : 35,000"); 
    }
    else  if(StrUID == "291CCF59"){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Name: Ala");
      lcd.setCursor(0,1);
      lcd.print("Balance : 70,000"); 
    }
    
  }else{
    Serial.println("Fail...");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wrong PIN...");

  }
}

void ScanPIN(){
  password.reset();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN...");

  keypad.waitForKey();// Read the key pin 1
  keypad.waitForKey();// Read the key pin 2
  keypad.waitForKey();// Read the key pin 3
  keypad.waitForKey();// Read the key pin 4
  keypad.waitForKey();// Read the key pin 4
}
