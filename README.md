# Grill Controller

An ESP866 acting as a soft access point serving an HTML page to allow remote control of a grill over a fire pit. An arduino nano receives serial data from the esp8266 to control the height of the grill via a stepper motor and a gearbox

  
# Hardware

* ESP8266 Wemos D1 mini
* Arduino nano
* MLX90614 infrared temperature sensor
* HC-SR04 ultrasonic range sensor
* Nema 23 Stepper motor
* TB6600 Stepper motor driver

## Behavior

- The ESP8266 will start up as a soft access point
- The nano will start up and connect to the ESP8266
- The nano will check the eeprom for the maximum and minimum positions and will set the mn and max variables accordingly
- If the eeprom is empty, the nano will use the default values
- The nano will then take a height reading from the ultrasonic sensor
- The nano will send the current height to the ESP8266
- The ESP8266 will then send the current height to the client
- The client will send a post request to 'update' every two seconds
- The esp8266 will send a message to the nano
- The nano will respond with the current temperature readings
- The ESP8266 will then send the current temperature readings to the client
### Calibration feature
  - The calibrate button on the HTML page opens the setminimum and set maximum buttons
  - The user will use the height slider to move the grill to lowest or highest position
  - The User will press the set minimum button or maximum button accordingly
    - This sends a post request to the esp8266
    - The esp8266 will send serial data to nano to trigger the set min or max function 
    - The nano will take a distance measurment and set the eeprom as well as the global variables accordingly
    - The nano will send the distance to the esp8266, which will send the data to the client
    - the minimum or maximum of the height slider will be updated 
 ## Future Features
  -Add PID control to maintain a set temperature of the grill by varying the height
