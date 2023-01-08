#include <EEPROM.h>

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    EEPROM.begin();
    Serial.println("Ready");
    Serial.println(EEPROM.read(0));
}

void loop()
{
    Serial.println(EEPROM.read(0));
    delay(1000);
}