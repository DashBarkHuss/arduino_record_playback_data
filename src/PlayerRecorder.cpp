/*
  PlayerRecorder.cpp - keeps sample rate and handles recording and playing back data for the dream phone.
  Created by Dash Bark-Huss, Jan 28, 2022.
  Released into the public domain but you need to say a nice thing to stranger if you use it.

  Questions still not :
  1. How to close file when recording before unpluging with out loosing data? do you just let it run a bit? 
  Right not we call close() in the example after a certain time.
  2. Can't use _fileName in some areas *search "firsttry.txt" to see where

*/
// #ifndef ?
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "PlayerRecorder.h"

PlayerRecorder::PlayerRecorder(bool record)
{

  _record = record;
  _sendData = false;
  _maxDataLineLength = 20; //(ex: 2800000,1000,1000\r\n\0) this should be max length of a data line plus null terminator (ex: 2800000,1000,1000\r\n\0)
  endOfData = false;
  _lastDataRecorded = false;
}

// ==============================================Record functions==========================================
// save data from sensor to buffer
// then save data to SD card

void PlayerRecorder::record(String dataString)
{

  _insertNextBufferDataLine(dataString);
};

void PlayerRecorder::_insertNextBufferDataLine(String dataString)
{
  // put data into input string
  // so that when full we send input string to sd card

  byte lastInd = 0;
  while (_inputString[lastInd] != '\0')
  {
    lastInd++;
  }

  byte ind = 0;
  while (dataString[ind] != '\0')
  {
    _inputString[lastInd + ind] = dataString[ind];
    ind++;
  }
  _inputString[lastInd + ind] = '\r';
  _inputString[lastInd + ind + 1] = '\n';
  _inputString[lastInd + ind + 2] = '\0';
}

bool PlayerRecorder::_dataBufferNeedsToSendToSD()
{
  byte index = 0;

  while (_inputString[index] != NULL && index < sizeof(_inputString))
  {
    index++;
  }

  if (index > sizeof(_inputString) - _maxDataLineLength)
  {
    return true;
  }
  return false;
}

void PlayerRecorder::_sendToSD()
{
  if (_sendData)
  {

    _myFile = SD.open(_fileName.c_str(), FILE_WRITE);

    _myFile.print(_inputString);
    _myFile.close();
    _arrayToNull(_inputString, sizeof(_inputString));
  }
}

void PlayerRecorder::deleteFile()
{
  Serial.println("deleting file.");

  // not working
  // SD.remove(_fileName.c_str());
  SD.remove("firsttry.txt");

  // not working
  // if (SD.exists(_fileName.c_str()))
  if (SD.exists("firsttry.txt"))
  {
    Serial.println("---ERROR DELETING----DATA.TXT exists.");
  }
}

// close _myFile manually
void PlayerRecorder::close()
{
  _myFile = SD.open(_fileName.c_str(), FILE_WRITE);

  _myFile.print(_inputString);
  _myFile.close();
  _arrayToNull(_inputString, sizeof(_inputString));
}

// ==============================================Playback functions==========================================
// // get data from SD card and save to buffer
// //

String PlayerRecorder::play()
{

  return _popNextBufferDataLine();
}

String PlayerRecorder::_popNextBufferDataLine()
{

  // put data line from inputString which is from
  //
  char data[_maxDataLineLength];
  _arrayToNull(data, sizeof(data));

  byte ind = 0;

  // save next line of _inputString to data
  while (_inputString[ind] != '\n' && _inputString[ind] != '\0')
  {
    data[ind] = _inputString[ind];
    ind++;
  }
  data[ind] = '\0';

  byte startMove = ind + 1;

  ind = 0;

  // clear line of _inputString that was saved to data
  while (ind < sizeof(_inputString))
  {
    char newChar = _inputString[ind + startMove];
    _inputString[ind] = newChar;
    ind++;
  }

  return data;
}

void PlayerRecorder::_saveFileToBuffer(byte maxChar, byte startFillingBufferFrom)
{
  byte numOfChar = 0 + startFillingBufferFrom;
  if (_myFile)
  {

    while (numOfChar < (maxChar - 1))
    {
      char newChar = _myFile.read();

      if (newChar == EOF)
      {
        _inputString[numOfChar] = '\0';
        _lastDataRecorded = true;
      }

      _inputString[numOfChar] = newChar;
      numOfChar++;
    }

    _inputString[maxChar - 1] = '\0';
  }
}

// PLAY find the first NULL character in _indexString if its lass than the next line taken
byte PlayerRecorder::_startPlace()
{
  // find the first NULL character in _indexString
  byte index = 0;

  while (_inputString[index] != NULL && index < sizeof(_inputString))
  {

    index++;
  }
  byte place = index;

  // if its less than the length of a data line
  if (index <= _maxDataLineLength)
  {

    return place;
  }

  return 0;
}

bool PlayerRecorder::_fileBufferNeedsToBeReset()
{
  byte index = 0;

  while (_inputString[index] != NULL && index < sizeof(_inputString))
  {

    index++;
  }

  // PLAY if its less than the length of a data line + 1
  // (or can be set to any number more than the length of a data line)  (maxdataline: 2800000,1000,1000\r\n\0 here null is at index 19)
  if (index <= _maxDataLineLength)
  {
    return true;
  }

  return false;
}

// ==============================================Shared functions==========================================

//  checks if buffer needs to be emptied to the SD (RECORD) or filled with the SD (PLAY)
void PlayerRecorder::checkBuffer()
{
  if (_record)
  {

    _sendData = _dataBufferNeedsToSendToSD();
    if (_sendData)
      _sendToSD();
  }
  else
  {
    if (endOfData)
      return;
    bool reset = _fileBufferNeedsToBeReset();

    if (_inputString[0] == NULL || reset || _inputString[0] == EOF)
    {
      if (_lastDataRecorded)
      {
        endOfData = true;
        Serial.println("PLAYBACK data ended-------");
        return;
      }

      byte bufStart = _startPlace();
      _saveFileToBuffer(sizeof(_inputString), bufStart);
    }
  }
}

void PlayerRecorder::_arrayToNull(char arr[], size_t sz)
{
  byte index = 0;
  while (index < 9)
  {
    arr[index] = '\0';
    index++;
  }
}

void PlayerRecorder::setUpFile(String fileName)
{
  _fileName = fileName.c_str();
  Serial.print("Initializing SD card...");
  // Open serial communications and wait for port to open:
  if (!Serial)
  {
    Serial.begin(115200);
    while (!Serial)
    {
    } // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");
  if (!SD.begin(5))
  {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  if (_record)
  {
    // _deleteFile(); optional but may accidentally lead to deleted recently recorded data
    _myFile = SD.open(_fileName.c_str(), FILE_WRITE);
  }
  else
  {
    _myFile = SD.open(_fileName.c_str());
  }
  if (!_myFile)
  {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}

// ==============================================Log functions==========================================

// prints all data in SD card file
void PlayerRecorder::dumpSD()
{
  _myFile = SD.open(_fileName.c_str());

  Serial.println("Printing entire SD file: ");
  if (_myFile)
  {
    while (_myFile.available())
    {
      Serial.write(_myFile.read());
    }
    _myFile.close();
  }
}