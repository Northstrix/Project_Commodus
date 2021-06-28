// Project Commodus
// Developed by Maxim Bortnikov
// Distributed under the MIT License
// For more information visit https://github.com/Northstrix/Project_Commodus
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 6, 5, 4, 3);  /* For 4-bit mode */
//LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3); /* For 8-bit mode */
String str;
String str2 = "if|qZ5uQw~}Eo~dm{PRypXE#oj:WHZ}w*Tn#@E2%Nc=qSs:lJtZ/[W,eAfG`i`'"; // Clear sequence
int i = 0;
void setup() {
  Serial.begin(115200);
  lcd.begin(24,2);  // Initialize 24x2 LCD
  lcd.clear();
}

void loop() {
  while (!Serial.available()) {}
  str = Serial.readString();
  if (i == 3){ // Fill the last 8 segment of the second row
  lcd.setCursor(16,1);
  lcd.print(str);
  i=0;
  }
  else if (i == 2){ // Fill the first 16 segments of the second raw
  lcd.setCursor(0,1);
  lcd.print(str);
  i++;
  }
  else if (i == 1){ // Fill the last 8 segments of the first display
  lcd.setCursor(16,0);
  lcd.print(str);
  i++;
  }
  else if (i == 0){ // Fill the first 16 segments of the first display
  lcd.setCursor(0,0);
  lcd.print(str);
  i++;
  }
  
  if (str == str2){ // Clear the LCD
  lcd.clear();
  i=0;
  }  
    
  lcd.write(1);
  delay(10);
}
