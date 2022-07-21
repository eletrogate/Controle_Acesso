#include <SPI.h>
#include <LiquidCrystal.h>
#include <MFRC522.h>
 
#define LCD_RS 7
#define LCD_E 6
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 4
#define LCD_D7 5
#define SDA_PIN  9
#define RST_PIN 8
#define CartaoLiberado " 22 C9 0A 10"
#define AcionaBuzzer 10
#define BeepsLiberado 2
#define BeepsNegado 4

MFRC522 mfrc522(SDA_PIN, RST_PIN);    // Create MFRC522 instance.
 
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
 
char st[20];
void mensageminicial();
String UID= "";
byte letra;
 
void setup() 
{
  pinMode(AcionaBuzzer,OUTPUT);
  Serial.begin(9600);    
  SPI.begin();        
  mfrc522.PCD_Init();    
  lcd.begin(16, 2);  
  StandardMessage();
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
  
  getUID();  //separa a string com o UID para ser testada

  if (UID == CartaoLiberado) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    //Mostra mensagem de acesso liberado  e aciona buzzer
    lcd.print("Acesso Liberado!");
    BuzzerBeeps(BeepsLiberado);
    delay(2000);
    StandardMessage();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    //Mostra mensagem de acesso negado e aciona buzzer
    lcd.print("Acesso Negado!");
    BuzzerBeeps(BeepsNegado);
    delay(2000);
    StandardMessage();
  }
  
  delay(1000);
} 

void StandardMessage()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Pronto para");  
  lcd.setCursor(0,1);
  lcd.print(" Leitura!");  
}

void getUID()
{
  Serial.print("UID da tag : ");
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     if( mfrc522.uid.uidByte[i] < 0x10)
       UID.concat(" 0");
     else
       UID.concat(" ");
      
    // conteudo.concat( String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     UID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  UID.toUpperCase();
  Serial.println(" ");
}

void BuzzerBeeps(int NumeroBeeps)
{
  int i;
  for(i=0; i<NumeroBeeps; i++)
  {
    digitalWrite(AcionaBuzzer, HIGH);
    delay(500);
    digitalWrite(AcionaBuzzer, LOW);
    delay(500);
  }
}