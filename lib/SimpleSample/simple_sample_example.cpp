#include <Arduino.h>
#include <SimpleSample.h>

SimpleSample sample(100); // You set sample time to desired ms

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{

  if (sample.isSampleTime())
  {
    // This is where you put whatever you want to do during
    // the sample time e.g.` collect data

    int reading = analogRead(A1);

    Serial.print("Time");
    Serial.print('\t');
    Serial.println("Data");

    Serial.print(sample.currentSampleTime);
    Serial.print('\t');
    Serial.println(reading);
  }
}
