
# `PlayerRecorder` and  `SimpleSample`
This project contains two libraries. `PlayerRecorder` to record and play back analogue data. And `SimpleSample`, to set a sample rate for collecting data.

`PlayerRecorder` helps you record analog data to an SD file. After the data is recorded, you can playback the data through your program. I created this so that I could record my sleep data and then play it back through some program when I'm awake in order develop and fine to tune my program to detect certain parts of sleep. Otherwise I had to develop my programs in real time. Not an easy task when you're asleep! 

### Basic Steps
1. Add `PlayerRecorder` to a program. 
2. Set  `PlayerRecorder` set to record (`true`). 
3. Run the program. 
4. Stop the program.
5. Upload the same program but this time with  `PlayerRecorder` set to playback (`false`).  
6. Run the program and you will have the same exact sequence of events in your program as when you recorded the data.

## Filename issue in SD functions
There's an issue in the `PlayerRecorder.cpp` library. I had to hardcode the data filename into parts of the library. In the platform.io IDE, I couldn't use the variable `_fileName` for `SD.remove("firsttry.txt")` or `SD.exists("firsttry.txt")`. So you have to change that to your file name or figure out how to use the variable `_fileName`.

Using the `_fileName` in `SD.remove(_fileName.c_str())` gave me this error:

`argument of type "const char *" is incompatible with parameter of type "char *"C/C++(167)`

### Try it

1. Get [platform.io](https://www.youtube.com/watch?v=dany7ae_0ks) to use arduino in VSCode.
2. Import this project following the video in #1.
3. Plug in arduino with SD card attached. I used Arduino Uno. You can connect some sensors to A0 and A1 to get some values to record.

    _SD card pins Arduino UNO_
   - CS: pin 5 (can be any pin in reality, but 5 is hard coded into this project `SD.begin(5)`)
   - MOSI: 11
   - MISO: 12
   - SCK: 13
   - GND: GND
   - 5v: 5v


4. Set `record = true` in `player_recorder_example.cpp` 
5. Upload and run `player_recorder_example.cpp` to your arduino to record.   
6. Set `record = false` in `player_recorder_example.cpp` 
7. Upload and run `player_recorder_example.cpp` to your arduino to playback the data you just recorded.



