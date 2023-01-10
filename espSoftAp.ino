

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

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
int maxHeight = 50;
int minHeight = 10;
ESP8266WebServer server(80);

void handleRoot()
{ /* this is the html page that is served to the client, it is in minified form to save space. The readable html can be seen in the index.html file and the javascript can be seen in the js.js file. I simply minified,copied and pasted the code from those files into this function.
     if you want to change the html or javascript, you can do so in the index.html and js.js files and then minify them using https://www.minifier.org/ and then copy and paste the minified code into this function. You will need to escape the double quotes with a backslash like this: \". I found it easier to use single quotes wherever I could '' instead of ""
     but some js functions require double quotes so I used those where necessary. */

    server.send(200, "text/html", "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width,initial-scale=1'><title>ESP Grill</title></head><style>button{width:120px;height:60px;border-radius:1rem;font-size:large;font-weight:700;margin-top:1rem;background-color:#ecebe0;padding:10px;border:solid;cursor:pointer}button:active{background-color:#298329}p{padding:.5rem;font-size:xx-large}.container{display:block;height:fit;width:80vw;background-color:#191970;color:#fff;padding:.5rem;border-radius:1em}h1{padding:.5rem;font-size:xx-large}.controlContainer{display:flex;flex-direction:column;margin-bottom:2em}input{-webkit-appearance:none;background-color:orange;height:2rem;top:0;right:0;outline:0}.buttonBox{display:flex;justify-content:center}</style><body style='display:flex;flex-direction:column;align-items:center;background-color:gray'><h1>Chippewa Kitchen Controller</h1><div class='container'><p>Ambient: <span id='ambient'>0</span></p><p>Grill: <span id='current'>0</span></p><div class='controlContainer'><p>Target: <span id='target'>300</span></p><input id='tempSlider' name='temp' type='range' min='180' max='700' value='350'><p style='padding:.5rem;font-size:xx-large'>Height: <span id='currentHeight'>20</span></p><input type='range' name='height' id='height' step='1' min='0' max='100' value='20'><div class='buttonBox'><button id='button' name='setHeight'>Set Height</button></div></div></div><button name='calibrate' style='margin-top:10rem'>Calibrate</button><div id='calibrateButtons' style='display:none'><button name='minimum'>Set Minmum</button> <button name='maximum'>Set Maximum</button></div><script>"
                                  "const HTTP=new XMLHttpRequest;let heightHolder=0;const baseURL='http://192.168.4.1';let viewFlag=!1;const calibrate=document.querySelector('button[name=\"calibrate\"]'),setHeight=document.querySelector('button[name=\"setHeight\"]'),setMinimum=document.querySelector('button[name=\"minimum\"]'),setMaximum=document.querySelector('button[name=\"maximum\"]'),calibrateButtonContainer=document.getElementById('calibrateButtons'),heightSlider=document.getElementById('height'),ambient=document.getElementById('ambient'),target=document.getElementById('target'),current=document.getElementById('current'),currentHeight=document.getElementById('currentHeight'),tempSlider=document.getElementById('tempSlider'),tempInput=document.getElementById('target');function postRequest(req,route){HTTP.open('POST',`${baseURL}/${route}/`);const body=JSON.stringify(req);HTTP.onload=()=>{let res=JSON.parse(HTTP.responseText);current.textContent=res.current,ambient.textContent=res.ambient,res.height&&(currentHeight.textContent=res.height,currentHeight.value=res.height,heightSlider.value=res.height),res.max&&heightSlider.setAttribute('max',res.max),res.min&&heightSlider.setAttribute('min',res.min)},HTTP.setRequestHeader('Content-Type','text/plain'),HTTP.send(body)}function update(){postRequest('0','update')}heightSlider.oninput=function(){currentHeight.innerHTML=this.value},calibrate.addEventListener('click',(function(){viewFlag=!viewFlag,calibrateButtonContainer.style.display=viewFlag?'flex':'none'})),setMaximum.addEventListener('click',(function(){postRequest('0','max')})),setMinimum.addEventListener('click',(function(){postRequest('0','min')})),tempSlider.oninput=function(){this.value>699?(target.setAttribute('style','color: red;'),target.innerHTML='MAX'):(target.setAttribute('style','color: white;'),target.innerHTML=this.value)},setHeight.onclick=function(){postRequest(heightSlider.value,'height')},setInterval(update,2e3);"
                                  "</script></body></html> ");
}
void handleMin() // handle min post request, sends a command 'm' to arduino
{
    Serial.println("m");
    server.send(200, "text/plain", "{\"min\":" + String(minHeight) + "}");
}
void handleMax() // handle max post request, sends a command 'M' to arduino
{
    Serial.println("M");
    server.send(200, "text/plain", "{\"max\":" + String(maxHeight) + "}");
}
int getValue(int args) // get value from post request
{
    String value;
    for (uint8_t i = 0; i < args; i++)
    {
        value += server.arg(i);
    }
    value.setCharAt(0, '0');
    return value.toInt();
}

void handleHeight() // handle height post request
{
    int value = getValue(server.args()); // get value from post request
    heightSetting = value;
    Serial.println("H"); // send message to arduino
    Serial.print(heightSetting);
    server.send(200, "text/plain");
}

void handleTemp() // handle temp post request
{

    String message = "{\"ambient\":" + String(ambient) + ",\"current\":" + String(current) + ",\"height\":" + String(height) + "}"; // make json object
    int value = getValue(server.args());
    tempSetting = value;
    server.send(200, "text/plain", message);
}
void handleUpdate() // sends current temp, ambient temp, and height
{
    Serial.println('T');
    String message = "{\"ambient\":" + String(ambient) + ",\"current\":" + String(current) + ",\"height\":" + String(height) + "}"; // make json object
    server.send(200, "text/plain", message);
}
void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");

    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", handleRoot);
    server.on("/temp/", handleTemp);
    server.on("/update/", handleUpdate);
    server.on("/height/", handleHeight);
    server.on("/max/", handleMax);
    server.on("/min/", handleMin);
    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();

    if (Serial.available() > 0) // check for messages from arduino nano on serial lines
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
            server.send(200, "text/plain", "{\"height\":" + String(height) + "}");
            Serial.read();
            break;
        }
        case 'M':
        {
            maxHeight = Serial.parseInt();
            server.send(200, "text/plain", "{\"max\":" + String(maxHeight) + "}");
        }
        case 'm':
        {
            minHeight = Serial.parseInt();
            server.send(200, "text/plain", "{\"min\":" + String(minHeight) + "}");
        }
        case 'T':
        {
            ambient = Serial.parseInt();
            Serial.read();
            break;
        }
        case 'G':
        {
            current = Serial.parseInt();
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