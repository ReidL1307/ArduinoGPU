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

String value="";

char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>Keyboard</title>
<style>
 div {
    text-align: center;
  }
  body {
      background-color: #040404;
  }
  button {
    background-color: black;
    color: white;
  }
  .mainContainer {
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translateY(-50%) translateX(-50%);
  }
</style>
</head>
<body>
  <div class="mainContainer">
      <div>
        <input type="textbox" id="text">
      </div>
      <div>
        <button id="btn">Submit</button>
      </div>
  </div>
</body>
<script>
var new_msg="";
const Http = new XMLHttpRequest();
const loop = () => {
  new_msg=document.getElementById(textbox).value;
  Http.open("GET", "http://192.168.11.4/endp?value=${new_msg}");
  Http.send();
}

setInterval(1, loop);
</script>
</html>
)=====";

char matrix[60][64];
String text = "";
bool f = true;
void loop() {
  
  server.handleClient();
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

void endp() {
    server.send(200,"text/plain",String(value));
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
  server.on("/endp", endp);
  server.begin();
}
