
#include <Adafruit_MLX90614.h>
#include <HCSR04.h>
#include "BasicStepperDriver.h"
#include <EEPROM.h>
#define MOTOR_STEPS 200
#define RPM 500
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000
#define DIR 8
#define STEP 9
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
UltraSonicDistanceSensor distanceSensor(6, 7);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float minHeight = 12;       // mininum allowed grill height
float maxHeight = 45;       // maximun allowed grill height
int heightRequest;          // setting for requested height
float currentHeight = 30;   // current measured grill height
float circumference = 12;   // circumference of winch wheel
int temp;                   // requested temp hold setting
int revolutions;            // holds revoluton variable
byte minHeightRegister = 0; // eeprom register
byte maxHeightRegister = 1; // eeprom register

void getHeight()
{
    currentHeight = distanceSensor.measureDistanceCm();
    currentHeight = currentHeight / 2.54; // convert to inches
}

int getRevolutions(float distance)
{
    // takes in distance
    // divides distance by curcumference
    // multiply by 30 for gear reduction
    // multiply by 360 degrees per rev
    float degreesToRotate;
    distance = (distance / circumference) * 30;
    degreesToRotate = distance * 360;

    return degreesToRotate;
}

void handleHeight(int heightRequest)
{
    if (heightRequest < minHeight)
    {
        heightRequest = minHeight;
    }
    if (heightRequest > maxHeight)
    {
        heightRequest = maxHeight;
    }
    getHeight();
    int distanceToMove = heightRequest - currentHeight;
    revolutions = getRevolutions(distanceToMove);
    stepper.rotate(revolutions);
    getHeight();
    Serial.print("H");
    Serial.print(currentHeight);
    Serial.println();
}
void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    if (!mlx.begin())
    {
        Serial.println("E");
    };
    stepper.begin(RPM);
    stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
    delay(500);
    if (EEPROM.read(minHeightRegister) != 255)
    {
        minHeight = EEPROM.read(0);
    }
    if (EEPROM.read(maxHeightRegister) != 255)
    {
        maxHeight = EEPROM.read(1);
    }
    Serial.println('M');
    Serial.print(EEPROM.read(maxHeightRegister));
    Serial.println();
    Serial.println('m');
    Serial.print(EEPROM.read(minHeightRegister));
}

void loop()
{

    if (Serial.available() > 0)
    {
        int incomingByte;
        incomingByte = Serial.peek();
        switch (incomingByte)
        {

        case 'H':
        {
            heightRequest = Serial.parseInt();
            handleHeight(heightRequest);
            Serial.read();
            break;
        }
        case 'M': // Max height identifier
        {
            maxHeight = distanceSensor.measureDistanceCm() / 2.54;
            EEPROM.write(maxHeightRegister, maxHeight);
            Serial.read();
            Serial.println('M');
            Serial.print(EEPROM.read(maxHeightRegister));
            Serial.println();
            break;
        }
        case 'm': // min height identifier
        {
            minHeight = distanceSensor.measureDistanceCm() / 2.54;
            EEPROM.write(minHeightRegister, minHeight);
            Serial.read();
            Serial.println('m');
            Serial.print(EEPROM.read(minHeightRegister));
            break;
        }
        case 'T':
        {
            Serial.println("G");
            Serial.print(mlx.readObjectTempF());
            Serial.println();
            Serial.println("A");
            Serial.print(mlx.readAmbientTempF());
            Serial.println();
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