#include <ESPVGAX2.h>
#include <fonts2/monodos8.h>

ESPVGAX2 vga;
char matrix[60][64];
String text = "";
bool f = true;
void loop() {
  while (1) {
    while (Serial.available()) {
      char c = Serial.read();

      
      if(f){        
        if(c!='\n') {
          text += c;
        } else {
          text += c;
          f = false;
        }
      } else {
        f = true;
      }
    }

    vga.print(text.c_str(), 0, 0, 0xf, true, sizeof matrix);
  }
}
void setup() { 
  Serial.begin(9600);      
  vga.begin();
  vga.setBitmapFont((uint8_t*)img2_monodos8_data, 8);
  memset(matrix, ' ', sizeof matrix);
}
