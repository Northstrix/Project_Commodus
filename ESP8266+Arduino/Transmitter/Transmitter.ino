// Project Commodus
// Developed by Maxim Bortnikov
// Distributed under the MIT License
// For more information visit https://github.com/Northstrix/Project_Commodus
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DES.h>
DES des;
// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x40, 0xF5, 0x20, 0x33, 0x9A, 0xF5};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  byte a[8];
  int pos;
  int line;
  int dispn;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
}

void encryptplt(String plaintext)
{
  byte in[] = {32, 32, 32, 32, 32, 32, 32, 32};
  for (int i = 0; i < 8; i++)
  {
      char x = plaintext[i];
      in[i] = int(x);
  }
  byte out[8];
  byte key[] = { 
                  0x3E, 0x0A, 0x6F, 0xB6, 0x1D, 0xA6, 0x88, 0x66,
                  0xDB, 0xCF, 0x4E, 0x5A, 0x31, 0xF5, 0x6A, 0x6E,
                  0xB8, 0x01, 0x9F, 0xCC, 0xBC, 0x06, 0x73, 0xFE,
                };
  
  //encrypt
  des.tripleEncrypt(out, in, key);
//  printArray(out);
  for (int i=0; i<8; i++){
  myData.a[i] = out[i];
  Serial.print(myData.a[i]);
  }
  }
void send_data(int line, int disp){
   String str;
   Serial.println("Enter the data to encrypt and send:");
   while (!Serial.available()) {}
   str = Serial.readString(); // Data to encrypt (8 characters)
   str+= "                ";
   int str_len = str.length() + 1; 
   char char_array[str_len];
   int i, k;
   char leftThird[8], rightThird[8];
   str.toCharArray(char_array, str_len); 
          for(i = 0; i < 8; i++) {
          leftThird[i]= char_array[i];
            }
          leftThird[i] = '\0';

          for(i = 8, k = 0; i < 16; i++, k++) {
          rightThird[k]= char_array[i];
           }

   encryptplt(leftThird); // Encryption function
   myData.pos = 0; // column 0
   myData.line = line; // row 0
   myData.dispn = disp;
   // Send the message via ESP-NOW
   esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   delay(24);
   encryptplt(rightThird); // Encryption function
   // Start displaying the data from
   myData.pos = 8; // column 0
   myData.line = line; // row 0
   myData.dispn = disp;
   // Send the message via ESP-NOW
   esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}
   
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
   // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}
 
void loop() {
  int x;
     while (x) {
        Serial.println("");
        Serial.println("What are you going to do?");
        Serial.println("1.Enter the data for the upper part of the first display");
        Serial.println("2.Enter the data for the bottom part of the first display");
        Serial.println("3.Clear the first display");
        Serial.println("4.Enter the data for the upper part of the second display");
        Serial.println("5.Enter the data for the bottom part of the second display");
        Serial.println("6.Clear the second display");
        Serial.println("7.Enter the data for the third display");
        Serial.println("8.Clear the third display");
        ///...
          while (!Serial.available()) {}
          x = Serial.parseInt();
       if (x == 1) {
        send_data(0,1); // upper part of the first display
       }
       
       if (x == 2) {
       send_data(1,1); // bottom part of the first display
       }
       
       if (x == 3) {
           myData.dispn = 200;
          // Send the message via ESP-NOW
          esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
       }
       
       if (x == 4) {
        send_data(0,2); // upper part of the second display
       }
       
       if (x == 5) {
       send_data(1,2); // bottom part of the second display
       }
       
       if (x == 6) {
           myData.dispn = 201;
          // Send the message via ESP-NOW
          esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
       }

       if (x == 7) {
       send_data(1,3); // bottom part of the second display
       }
       
       if (x == 8) {
           myData.dispn = 202;
          // Send the message via ESP-NOW
          esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
       }
       ///...
     
  }
}
