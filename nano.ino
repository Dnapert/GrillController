
#include <Adafruit_MLX90614.h>
#include <HCSR04.h>
#include "BasicStepperDriver.h"
#define MOTOR_STEPS 200
#define RPM 500
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000
#define DIR 8
#define STEP 9
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
UltraSonicDistanceSensor distanceSensor(6, 7);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int minHeight = 0;        // mininum allowed grill height
int maxHeight = 100;      // maximun allowed grill height
int heightRequest;        // setting for requested height
int currentHeight = 30;   // current measured grill height
float circumference = 12; // circumference of winch wheel
int temp;                 // requested temp hold setting
int revolutions;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    // Serial.println("Ready");
    if (!mlx.begin())
    {
        Serial.println("E");
    };
    stepper.begin(RPM);
    stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
}

void getHeight()
{
    currentHeight = distanceSensor.measureDistanceCm();
    // currentHeight = currentHeight / 2.54;
    // Serial.println("height" + String(currentHeight));
    // Serial.println();
}

int getRevolutions(float distance)
{
    // takes in distance
    // divides distance by curcumference
    // multiply by 30 for gear reduction
    // multiply by 360 degrees per rev
    float degreesToRotate;
    distance = (distance / circumference) * 30;
    // Serial.println("distance" + String(distance));
    degreesToRotate = distance * 360;
    // Serial.println("degrees to rotate" + String(degreesToRotate));

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
    // getHeight();
    int distanceToMove = heightRequest - currentHeight;
    // Serial.println("distance to move" + String(distanceToMove));
    revolutions = getRevolutions(distanceToMove);
    // Serial.println("revs" + String(revolutions));
    stepper.rotate(revolutions);
    getHeight();
    // Serial.println("currentheight" + String(currentHeight));
    Serial.print("H");
    Serial.print(currentHeight);
    Serial.println();
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
        default:
        {
            Serial.read();
            break;
        }
        }
    }
    Serial.print("C");
    Serial.print(mlx.readObjectTempF());
    Serial.println();
    delay(1000);
    Serial.print("A");
    Serial.print(mlx.readAmbientTempF());
    Serial.println();
    delay(1000);
}