# control de acceso con RFID
### Este es un proyecto de un control de acceso con un lector RFID, utilizando un ESP32 como microcontrolador.
![image](https://github.com/user-attachments/assets/1d6127e5-369b-440c-807f-708e8dc6c3f2)
![image](https://github.com/user-attachments/assets/a8055412-127c-48c3-94c0-14702be54389)

### Los componentes utilizados son:
1. RFID RC522
2. ESP32 dev kit v1
3. lcd display 16x5
4. Placa 10x7cm
5. Buzzer
6. Regulador 9v-5v L7805ACD2T
7. Rele 9v PC817
8. Keypad 4x4
9. bornera de 2
10. bornera de 3
11. 4 Resitencias SMD de ohms

### Para este proyecto, utilizamos un servidor para guardar la informacion de cada persona y tarjeta. asi se puede identificar cuando el RFID lee una tarjeta, si el codigo esta en la base de datos, se le permitira el acceso. de lo contrario, lo rechaza. En la variable "SSID" y "PASSWORD" se coloca a que red se quiere conectar el esp32, y la contraseña para asi estar conectado con el servidor.
```
#include <WiFi.h>
#include <HTTPClient.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);  

const char* ssid = "Estudiantes";
const char* password = "educar_2018";
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
 wifi();
 Pines();
 iniSPI();
 iniRFID();
}
void loop() 
 {
  {
  char key = keypad.getKey();
  if (key) {
    
    ingresaPass[indice]=key;
   
    indice++;
    
    LCD. print(key);
  
  }
   if(indice==6){
    
    if(strcmp(pass,ingresaPass)==0){
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.println("Acceso permitido");
      digitalWrite (pinrelay , HIGH);
      delay(3000);
       LCD.clear();
       return ; 
    }
    
    else{
       digitalWrite(pinbuzzer, HIGH );
       LCD.clear();
      LCD.setCursor(0, 0);
       LCD.println("Acceso denegado");
      delay(3000);
      LCD.clear();
      return;
    }
    
    indice=0;
    delay(2500);
    LCD.clear();
  }
  
   
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
  if(content.substring(1)== "E3 8D 59 FA"){
    if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://127.0.0.1/prueba1/index.php";
    String id = "E3 8D 59 FA";

    http.begin(url); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "id=" + id;

    int respuesta = http.POST(postData); 
    if (respuesta == HTTP_CODE_OK) {
      String cuerpo_respuesta = http.getString();
      if(cuerpo_respuesta == "ID encontrada"){
    LCD.init();
    LCD.backlight();
    LCD.print("Acceso autorizado");
    delay(1100);
    LCD.clear();
    delay(1000);
     http.end();
  }
    }
    }else {
    digitalWrite(pinbuzzer , HIGH);
     LCD.print("Acceso denegado");
    delay(1500);
    return;
  }
   
  LCD.init();
  LCD.backlight();
  LCD.print("bienvenido PEPE");
  LCD.setCursor(0, 1);
  LCD.print("DNI: 22567843");
  delay(2500);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("ingrese la");
  LCD.setCursor(0, 1);
  LCD.print("contrasena:");
  delay(2500);
  LCD.clear();
  }


 }
 void wifi(){
  WiFi.begin(ssid, password);
 }
 void Pines() {
  pinMode(pinbuzzer, OUTPUT);
  pinMode(pinrelay, OUTPUT);
  digitalWrite(pinbuzzer, LOW);
  digitalWrite(pinrelay, LOW);
}

void iniSPI() {
  SPI.begin();
}

void iniRFID() {
  mfrc522.PCD_Init();
}
```
