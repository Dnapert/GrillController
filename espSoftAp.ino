

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#ifndef STASSID
#define STASSID "mynet"
#define STAPSK "password"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;
int tempSetting = 300;
int ambient;
int current;
int heightSetting = 50;
int height;

ESP8266WebServer server(80);

// mlx.readAmbientTempF();
// mlx.readObjectTempF();

/*void readtemp(){
    ambient = mlx.readAmbientTempF();
    current = mlx.readObjectTempF();
  }*/

void handleRoot()
{

    server.send(200, "text/html", "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width,initial-scale=1'><title>ESP Grill</title></head><body style='display:flex;flex-direction:column;align-items:center;background-color:gray'><h1 style='align-self:center;color:#fff'>Chippewa Kitchen Controller</h1><div style='display:block;height:fit;width:80vw;background-color:#191970;color:#fff;padding:.5rem;border-radius:1em'><p style='padding:.5rem;font-size:xx-large'>Ambient: <span id='ambient'>0</span></p><p style='padding:.5rem;font-size:xx-large'>Grill: <span id='current'>0</span></p><div style='display:flex;flex-direction:column;margin-bottom:2em'><p style='padding:.5rem;font-size:xx-large'>Target: <span id='target'>300</span></p><input style='-webkit-appearance:none;background-color:orange;height:2rem;top:0;right:0;outline:0' id='tempSlider' name='temp' type='range' min='180' max='700' value='350'><p style='padding:.5rem;font-size:xx-large'>Height: <span id='currentHeight'>0</span></p><input type='range' name='height' id='height' style='-webkit-appearance:none;background-color:orange;height:2rem;top:0;right:0;outline:0' step='1' min='0' max='100' value='20'></div></div><script>const heightSlider = document.getElementById('height');"
                                  "const ambient = document.getElementById('ambient');"
                                  "const target = document.getElementById('target');"
                                  "const current = document.getElementById('current');"
                                  "const currentheight = document.getElementById('currentHeight');"
                                  "heightSlider.oninput = function() {"
                                  "currentheight.innerHTML = this.value;"
                                  "postRequest(this.value,'height');"
                                  " };"
                                  "const tempSlider = document.getElementById('tempSlider');"
                                  "const tempInput = document.getElementById('target');"
                                  "tempSlider.oninput = function() {"
                                  "if(this.value > 699){"
                                  "target.setAttribute('style', 'color: red;');"
                                  "target.innerHTML = 'MAX';"
                                  "console.log(target.innerHTML);"
                                  "}"
                                  "else{ "
                                  "target.setAttribute('style', 'color: white;');"
                                  "target.innerHTML = this.value; "
                                  "}"
                                  " postRequest(this.value,'temp');"
                                  "};"
                                  "const HTTP = new XMLHttpRequest();"
                                  "const baseURL = 'http://192.168.4.1';"
                                  "function postRequest(req,route){"
                                  "HTTP.open('POST', `${baseURL}/${route}/`);"
                                  "const body = JSON.stringify(req);"
                                  "HTTP.onload = () => {"
                                  "let res = JSON.parse(HTTP.responseText);"
                                  "console.log(HTTP.responseText);"
                                  "current.textContent = res.current;"
                                  "ambient.textContent = res.ambient;"
                                  "currentHeight.textContent = res.height;"
                                  "};"
                                  "HTTP.setRequestHeader('Content-Type', 'text/plain');"
                                  "HTTP.send(body);"
                                  "};"
                                  "function update(){postRequest(0,'update')};"
                                  "setInterval(update,2000);"
                                  "</script></body></html>");
}

int getValue(int args)
{
    String value;
    for (uint8_t i = 0; i < args; i++)
    {
        value += server.arg(i);
    }
    value.setCharAt(0, '0');
    return value.toInt();
}

void handleHeight()
{
    int value = getValue(server.args());
    heightSetting = value;
    Serial.println("H" + String(heightSetting));
    server.send(200, "text/plain");
}

void handleTemp()
{
    String message = "{\"ambient\":" + String(ambient) + ",\"current\":" + String(current) + ",\"height\":" + String(height) + "}";
    int value = getValue(server.args());
    tempSetting = value;
    server.send(200, "text/plain", message);
}
void handleUpdate()
{
    String message = "{\"ambient\":" + String(ambient) + ",\"current\":" + String(current) + ",\"height\":" + String(height) + "}";
    server.send(200, "text/plain", message);
}
void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", handleRoot);
    server.on("/temp/", handleTemp);
    server.on("/update/", handleUpdate);
    server.on("/height/", handleHeight);
    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();
    if (Serial.available() > 0)
    {
        int incomingByte;
        incomingByte = Serial.peek();
        switch (incomingByte)
        {
        case 'A':
        {
            ambient = Serial.parseInt();
            Serial.read();
            break;
        }
        case 'C':
        {
            current = Serial.parseInt();
            Serial.read();
            break;
        }
        case 'H':
        {
            height = Serial.parseInt();
            Serial.read();
            break;
        }
        default:
        {
            Serial.read();
            break;
        }
        }
    }
}