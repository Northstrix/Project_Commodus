// Project Commodus
// Developed by Maxim Bortnikov
// Distributed under the MIT License
// For more information visit https://github.com/Northstrix/Project_Commodus
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DES.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 24, 2); // (address, columns, rows)
LiquidCrystal_I2C secondlcd(0x26, 24, 2); // (address, columns, rows)
///...
DES des;
// Structure example to receive data
// Must match the sender structur e
typedef struct struct_message {
  byte a[8];
  int pos;
  int line;
  int dispn;
} struct_message;

// Create a struct_message called myData
struct_message myData;
  void decryptplt(byte ct[],int pos,int line, int d){
  //decrypt
  Serial.print("Decrypt...");
  byte out[8];
  byte key[] = { 
                  0x3E, 0x0A, 0x6F, 0xB6, 0x1D, 0xA6, 0x88, 0x66,
                  0xDB, 0xCF, 0x4E, 0x5A, 0x31, 0xF5, 0x6A, 0x6E,
                  0xB8, 0x01, 0x9F, 0xCC, 0xBC, 0x06, 0x73, 0xFE,
                };
              
  des.tripleDecrypt(out, ct, key);
    for (int i = 0; i < 8; i++)
      {
    char x = out[i];
    Serial.print(x);
    if (d == 1){
    lcd.setCursor(i + pos, line);      
    lcd.print(x);
    }
    if (d == 2){
    secondlcd.setCursor(i + pos, line);      
    secondlcd.print(x);
    }
    ///...
      }
    Serial.println();
//    printArray(out);
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  int p = myData.pos;
  int l = myData.line;
  int d = myData.dispn;
  if (d == 200)
  lcd.clear(); // clear the first LCD
  if (d == 201)
  secondlcd.clear(); // clear the second LCD
  ///...
  byte out[8];
  for (int i = 0; i<8; i++){
  out[i] = myData.a[i];
  }
  if (d != 200 && d != 201) ///...
  decryptplt(out,p,l,d);
  Serial.println();
}
 
void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");
  lcd.begin(24,2);
  lcd.init(); // Initialise display
  lcd.backlight(); // Turn on the backlight (if you have it).
  secondlcd.begin(24,2);
  secondlcd.init(); // Initialise the second display
  secondlcd.backlight(); // Turn on the backlight (if you have it).
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
