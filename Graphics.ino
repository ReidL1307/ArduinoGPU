#include <ESPVGAX2.h>
#include <fonts2/monodos8.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
const char* ssid     = "ReidWifi";
const char* password = "Scott 1314";

IPAddress ip(192,168,11,4);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,255,255,0);
ESPVGAX2 vga;

char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>Keyboard</title>
<style>
</style>
</head>
<body>
</body>
<script>
</script>
</html>
)=====";

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

  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  server.on("/",[](){server.send(200,"text/html", webpage);});
}
