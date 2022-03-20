/*
  SimpleSample.h - keeps sample rate and handles recording and playing back data for the dream phone.
  Created by Dash Bark-Huss, Jan 28, 2022.
  Released into the public domain but you need to say a nice thing to stranger if you use it.*/
#include <Arduino.h>
#ifndef SimpleSample_h
#define SimpleSample_h
#include <SPI.h>
#include <SD.h>

#include "Arduino.h"
class SimpleSample
{
public:
  SimpleSample(long samplingDelta);
  bool handleData;
  bool isSampleTime();
  unsigned long currentSampleTime;

private:
  unsigned long _samplingDelta;
  unsigned long _loops;
  unsigned long _startTime;
};

#endif
