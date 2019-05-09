/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Simple Accelerometer Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;


int flag = 0;

unsigned long peakt [3];
unsigned long frequency = 0;
unsigned long startTime = 0;
unsigned long currentTime = 0;
float posmax, x, y, z,pos, prevpos = 0;
double sqrtpos10 = 0;
int counter = 0;
int firstTime = 0;

unsigned long currentMillis, previousMillis = 0;
 
bool ledState = LOW;

void setup() 
{
  Serial.begin(9600);

 // Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    //Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  pinMode(5, OUTPUT);
  // If you want, you can set accelerometer offsets

  /*
  mpu.setAccelOffsetX(0);
  mpu.setAccelOffsetY(0);
  mpu.setAccelOffsetZ(0);
  */
  checkSettings();
}

void checkSettings()
{
  //Serial.println();
  
  //Serial.print(" * Sleep Mode:            ");
  //Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  mpu.getSleepEnabled();
  //Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     /*Serial.println("Stops the clock and keeps the timing generator in reset");*/ break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: /*Serial.println("PLL with external 19.2MHz reference");*/ break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: /*Serial.println("PLL with external 32.768kHz reference");*/ break;
    case MPU6050_CLOCK_PLL_ZGYRO:      /*Serial.println("PLL with Z axis gyroscope reference");*/ break;
    case MPU6050_CLOCK_PLL_YGYRO:      /*Serial.println("PLL with Y axis gyroscope reference");*/ break;
    case MPU6050_CLOCK_PLL_XGYRO:      /*Serial.println("PLL with X axis gyroscope reference");*/ break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  /*Serial.println("Internal 8MHz oscillator");*/ break;
  }
  
  //Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            /*Serial.println("+/- 16 g");*/ break;
    case MPU6050_RANGE_8G:             /*Serial.println("+/- 8 g");*/ break;
    case MPU6050_RANGE_4G:             /*Serial.println("+/- 4 g");*/ break;
    case MPU6050_RANGE_2G:             /*Serial.println("+/- 2 g");*/ break;
  }  
/*
  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  
  Serial.println();
  */
}

void loop()
{
  currentTime = millis();
  Vector normGyro = mpu.readNormalizeGyro();
  Vector normAccel = mpu.readNormalizeAccel();
  
  pos = (abs(normAccel.XAxis) + abs(normAccel.YAxis) + abs(normAccel.ZAxis));
  //pos = pos-prevpos;
 
  sqrtpos10 = ((pos));

 if ((sqrtpos10 > 1) && flag == 0)
 {
  flag = 1;
  peakt[(counter)]=currentTime;
  counter++;
  }
  
  if ((pos>posmax) && flag == 1)
  {
    posmax=pos;
    peakt[(counter)]=currentTime;
  }
  if(sqrtpos10 < 0)
 {
  flag = 0;
  }

  
  Serial.print(sqrtpos10);
  Serial.print(",");
  /*
if (peakt[2]>peakt[1] && peakt[1]>peakt[0])//2 highest 0 lowest
{
  frequency = 0.5*(peakt[2]-peakt[0]);
}
else if (peakt[2]>peakt[0] && peakt[0]>peakt[1])//2 highest 1 lowest
{
  frequency = 0.5*(peakt[2]-peakt[1]);
}
else if (peakt[0]<peakt[1] && peakt[0]>peakt[2])//1 highest 2 lowest
{
  frequency = 0.5*(peakt[1]-peakt[2]);
}
 else if (peakt[2]<peakt[1] && peakt[2]>peakt[0])//1 highest 0 lowest
{
  frequency = 0.5*(peakt[1]-peakt[0]);
}
else if (peakt[1]<peakt[0] && peakt[1]>peakt[2])//0 highest 2 lowest
{
  frequency = 0.5*(peakt[0]-peakt[2]);
}
 else if (peakt[2]<peakt[0] && peakt[2]>peakt[1])//0 highest 1 lowest
{
  frequency = 0.5*(peakt[0]-peakt[1]);
}
*/

 if((counter > 2)&&(flag == 0))
 {
  frequency = 0.5*((peakt[1]-peakt[0])+(peakt[2]-peakt[1]));
  startTime = currentTime;
  counter = 0;
  peakt[0] = 0;
  peakt[1] = 0;
  peakt[2] = 0;
  }

  if(currentTime-peakt[0]>1000)
  {
  counter = 0;
  peakt[0] = 0;
  peakt[1] = 0;
  peakt[2] = 0;
  }

  if(currentTime - startTime > 1000)
  {
    frequency = 0;
    digitalWrite(4,LOW);
   }
  if (frequency != 0)
  {
     if (currentTime - previousMillis >= frequency) 
      {
        // save the last time you blinked the LED
        previousMillis = currentTime;
    
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) 
        {
          ledState = 1;
        } 
        else 
        {
          ledState = 0;
        }
        digitalWrite(4, ledState);
      }
}
  Serial.println(ledState);
  //Serial.print("frequency:");
 // Serial.println(frequency);
}


