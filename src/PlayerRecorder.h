/*
  PlayerRecorder.h - keeps sample rate and handles recording and playing back data for the dream phone.
  Created by Dash Bark-Huss, Jan 28, 2022.
  Released into the public domain but you need to say a nice thing to stranger if you use it.*/
#include <Arduino.h>
#ifndef PlayerRecorder_h
#define PlayerRecorder_h
#include <SPI.h>
#include <SD.h>

#include "Arduino.h"
class PlayerRecorder
{
public:
  PlayerRecorder(bool record);
  // Shared
  void checkBuffer();
  void setUpFile(String fileName);
  //record
  void record(String);
  void close();
  void deleteFile();
  // play
  String play();
  bool endOfData;
  // log
  void dumpSD();

private:
  // Shared
  int _BUF_DIM;
  bool _record;
  File _myFile;
  void _arrayToNull(char arr[], size_t sz);
  byte _maxDataLineLength;
  // record
  String _fileName;
  char _inputString[100];
  bool _dataBufferNeedsToSendToSD();
  void _sendToSD();
  void _insertNextBufferDataLine(String dataString);
  bool _sendData;
  // play
  String _popNextBufferDataLine();
  bool _fileBufferNeedsToBeReset();
  bool _lastDataRecorded;
  byte _startPlace();
  void _saveFileToBuffer(byte maxChar, byte startFillingBufferFrom);
};

#endif
