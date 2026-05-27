#include <LiquidCrystal_I2C.h>

/////////////////////VARIABLES AND QUERIES //////////////////////////////
#define RX2 16
#define TX2 17

byte npkQuery[8] = {0x01,0x03,0x00,0x1E,0x00,0x03,0x65,0xCD};
byte phQuery[8] = {0x01,0x03,0x00,0x06,0x00,0x01,0x64,0x0B};

uint16_t n=0,p=0,k=0;
float ph=0;
//

///////////////////////LCD AND SETUP///////////////////////
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}
//

//////////////////////////MAIN LOOP//////////////////////////////
void loop() {
  readNPK();
  readPH();

  display();

  delay(2000);
}
//

////////////////////////FUNCTIONS//////////////////////////////
void readNPK(){
  clearBuffer();
  Serial2.write(npkQuery,sizeof(npkQuery));
  delay(500);

  if(Serial2.available() >= 11){
    byte npk[11]={};
    Serial2.readBytes(npk,sizeof(npk));

    n = (npk[3]<<8) | npk[4];
    p = (npk[5]<<8) | npk[6];
    k = (npk[7]<<8) | npk[8];
  }
}

void readPH(){
  clearBuffer();
  Serial2.write(phQuery,sizeof(phQuery));
  delay(500);

  if(Serial2.available() >= 7){
    byte phres[7]={};
    Serial2.readBytes(phres,sizeof(phres));

    uint16_t phX100 = (phres[3]<<8) | phres[4];

    ph = phX100 / 100.0;
  }
}

void clearBuffer(){
  while(Serial2.available()) Serial2.read();
}

void display(){
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("N:");
  lcd.print(n);
  lcd.print(", P:");
  lcd.print(p);
  lcd.print(", K:");
  lcd.print(k);

  lcd.setCursor(0,1);

  lcd.print("PH:");
  lcd.print(ph);
}
//