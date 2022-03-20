#include <Arduino.h>
#include "../SimpleSample/SimpleSample.h"
#include <PlayerRecorder.h>
bool isRecording = false;               // false to play back recorded data, true to record
PlayerRecorder dataCorder(isRecording); // You set sample time to desired ms
SimpleSample sample(20);                // You set sample time to desired ms

void handleData(String data)
{
  Serial.print("handle data: ");
  Serial.println(data);
}

void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }
  dataCorder.setUpFile("firsttry.txt");

  // =====WARNING: below is only for testing. Remove this code when recording real data.======
  // only deleteFille() during testing.
  // If you leave deleteFille() when recording real data you will delete
  // the data that you recorded as soon as you turn your arduino on again.
  if (isRecording)
    dataCorder.deleteFile();
  // =====================WARNING=================
}

void loop()
{

  //  checks if buffer needs to be emptied to the SD (RECORD) or filled with the SD (PLAY)
  dataCorder.checkBuffer();

  if (sample.isSampleTime())
  {
    String dataLine;
    if (isRecording)
    {
      int reading = analogRead(A0);
      int reading2 = analogRead(A1);
      dataLine = String(millis()) + "," + String(reading) + "," + String(reading2);
      //  RECORD
      dataCorder.record(dataLine);
    }
    else
    {
      //  PLAY
      if (dataCorder.endOfData)
        return;

      dataLine = dataCorder.play();
    }
    handleData(dataLine);
  }

  // Code below is used to stop recording in a testing environment
  // In the real world turning off the arduino could stop recording
  // But when you plug it back in it will start recording again,
  // so you may want to add a button to start the recording process and code that in as well
  if (isRecording && millis() > 3000)
  {
    dataCorder.close();  // without this a few of the last data points wont be saved
    dataCorder.dumpSD(); // optional if you want to see what your whole sd file looks like
    delay(10000000);
  }
}
