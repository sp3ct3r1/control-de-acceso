#include <WiFi.h>
#include <HTTPClient.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);  

int tiempo;
const char* ssid = "MovistarFibra-A9B820";
const char* password = "nk58sgmCjzJ7GgTKo8q";
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' },
};

uint8_t rowPins[ROWS] = { 13, 12, 14, 27 };
uint8_t colPins[COLS] = { 26, 25, 33, 35 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
 
char pass[7]="654322";
char ingresaPass[7];
int indice=0;
int pinbuzzer = 15;
int pinrelay = 32;
void setup() 
{
tiempo = millis();
 Pines(); 
 wifi();
 iniSPI();
 iniRFID();
}
void loop() 
 {
  //control del keypad
  {
  char key = keypad.getKey();
  if (key) {
    
    ingresaPass[indice]=key;
   
    indice++;
    
    LCD. print(key);
  
  }
   //control de la contraseña
   if(indice==6){
    
    if(strcmp(pass,ingresaPass)==0){
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.println("Acceso permitido");
      digitalWrite (pinrelay , HIGH);
      if (millis()- tiempo > 5000){
      digitalWrite (pinrelay , LOW);
       LCD.clear();
       tiempo = millis();
       }
    }
    
    else
    {
      digitalWrite(pinbuzzer, HIGH );
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.println("Acceso denegado");
      if (millis()- tiempo > 3000){
      LCD.clear();
      tiempo = millis();
      }
      return;
    }
    
    indice=0;
    if (millis()- tiempo > 2000){
    LCD.clear();
    tiempo = millis();
    }
  }
  
   //leer tarjeta rfid
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
    
  }
 
  content.toUpperCase();
  //vemos si la id está registrada 
  if (content.substring(1)== "E3 8D 59 FA" ){
    if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://127.0.0.1/prueba1/index.php";
    String id = "E3 8D 59 FA";

    http.begin(url); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "id=" + id;
    //enviamos la id a la base de datos
    int respuesta = http.POST(postData); 
    if (respuesta == HTTP_CODE_OK) {
      String cuerpo_respuesta = http.getString();
      if(cuerpo_respuesta == "ID encontrada"){
    LCD.init();
    LCD.backlight();
    LCD.print("Acceso autorizado");
    if (millis()- tiempo > 2000){
    LCD.clear();
    tiempo = millis();
    }
    http.end();
  }
  }
  }
  } if (content.substring(1) == "A3 02 3F FC") 
  {  
  digitalWrite(pinbuzzer , HIGH);
  if (millis()- tiempo > 3000){
    LCD.init();
  LCD.backlight();
    LCD.print("Acceso denegado");
    tiempo = millis();
  }
   if (millis()- tiempo > 3000){
    LCD.clear();
    Serial.println(" Acceso denegado");
    tiempo = millis();
    }
    digitalWrite(pinbuzzer , LOW);
    return ;
  }
   
  LCD.init();
  LCD.backlight();
  LCD.print("bienvenido PEPE");
  LCD.setCursor(0, 1);
  LCD.print("DNI: 22567843");
  if (millis()- tiempo > 3000){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("ingrese la");
  LCD.setCursor(0, 1);
  LCD.print("contrasena:");
  tiempo = millis();
  }
  if (millis()- tiempo > 2000){
  LCD.clear();
  tiempo = millis();
  }
  }


 }
 //conexión a wifi
 void wifi(){
  WiFi.begin(ssid, password);
 }
 //configuracion de los pines
 void Pines() {
  pinMode(pinbuzzer, OUTPUT);
  pinMode(pinrelay, OUTPUT);
  digitalWrite(pinbuzzer, LOW);
  digitalWrite(pinrelay, LOW);
}
//iniciamos el lector de tarjeta
void iniSPI() {
  SPI.begin();
}

void iniRFID() {
  mfrc522.PCD_Init();
}
