#include "I2C.h"

//I2C i2c(SDA, SCL);
I2C i2c;

void setup() {   
    Serial.begin(9600);
    
    //arg: deviceId, regIndex, value
    i2c.Send(0x32, 0x40, 3); 
    
    //arg: deviceId, regIndex 
    //ret: value
    Serial.print(i2c.Receive(0x32, 0x40)); 
}

void loop()
{
    
}

