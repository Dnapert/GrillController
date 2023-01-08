# Grill Controller

An ESP866 acting as a soft access point serving an HTML page to allow remote control of a grill over a fire pit

  
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
- Every 2 secondsd the nano will take a temperature reading from the MLX90614 and send it to the ESP8266
- The client will send a post request to 'update' every two seconds
- The ESP8266 will then send the current temperature to the client
