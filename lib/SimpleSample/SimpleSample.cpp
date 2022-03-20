/*
  SimpleSample.cpp - keeps sample rate and handles recording and playing back data for the dream phone.
  Created by Dash Bark-Huss, Jan 28, 2022.
  Released into the public domain but you need to say a nice thing to stranger if you use it.
*/

#include "Arduino.h"
#include "SimpleSample.h"

SimpleSample::SimpleSample(long samplingDelta)
{
  currentSampleTime = 0;
  _samplingDelta = samplingDelta;
  _startTime = millis();
  _loops = 0;
}

bool SimpleSample::isSampleTime()
{
  unsigned long actualTime = millis();

  bool timeToSample = actualTime - _startTime >= _samplingDelta;
  if (timeToSample)
  {
    _loops = _loops + 1;
    currentSampleTime = actualTime;
    // currentSampleTime = _startTime + _samplingDelta

    _startTime = _startTime + _samplingDelta;
  }

  return timeToSample;
}