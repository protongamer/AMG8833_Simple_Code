#include <Wire.h>

uint8_t buf[128];
uint16_t v;
uint8_t i = 0;
float temp;
uint8_t btemp[128];
int n;




float toFloat(uint16_t val);

void setup() {

  Wire.begin(0x69);
  Serial.begin(115200);
  while(!Serial);
  delay(500);
  Wire.beginTransmission(0x00);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();


  Wire.beginTransmission(0x69);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  delayMicroseconds(10);
}

void loop() {


  /*if (Serial.available() > 0)
  {
    if (Serial.read() == 'p')
    {*/
      i = 0;
      while (i < 128)
      {
        Wire.beginTransmission(0x69);
        Wire.write(0x80 + i);
        Wire.endTransmission();
        delayMicroseconds(500);
        Wire.requestFrom(0x69, 32);
        delayMicroseconds(500);

        while (Wire.available())
        {
          buf[i] = Wire.read();
          i++;
        }
      }
      n = 1;
      for (i = 0; i < 128; i += 2)
      {
        v = (buf[i+1] << 8) | buf[i];
        temp = toFloat(v);
        btemp[i] = map(temp, 0.0, 125.0, 0, 254);
        Serial.print(temp);
        Serial.print(" ");

        if((n%8) == 0)
        {
          Serial.println();
        }
      
        n++;
      }
      Serial.println();
      //Serial.write(btemp, 128);
      //Serial.write(0xFF);

      //Serial.println("Done");
    /*}
  }*/

  delay(100);
}





float toFloat(uint16_t val)
{

  uint16_t mask = 0x100;
  uint8_t negVal = 0;
  float fbit = 64.0;
  float temperature = 0;

  //val = (buf[1] << 8) | buf[0];

  if ((val & 0xE00) == 0xE00)
  {
    negVal = 1;
    val = ((~val) & 0x1FF) + 1;
  }

  while (mask != 0)
  {
    if ((val & mask) == mask)
    {
      temperature += fbit;
    }
    fbit /= 2;
    mask >>= 1;
  }
  temperature = (negVal == 1) ? -temperature : temperature;
  return temperature;
}
