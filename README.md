# DIY Alexa ESP32 Firmware

This folder contains the firmware for running DIY Alexa on the ESP32.

We are using Platform.io to build the firmware.

If you're having problems detecting the wakeword `Marvin` then you can try lowering the detection threshold:

`/src/state_machine/DetectWakeWordState.cpp` line 59. and change % that what you want.


## Config options

To set things up for yourself, edit the `config.h` file and fill in your WiFi details.

There are a number of options in this file that you can modify to suit your own setup.

```
// are you using an I2S microphone - comment this out if you want to use an analog mic and ADC input
#define USE_I2S_MIC_INPUT
```
If you are using an I2S Microphone then you need to tell the system which channel you have configure the microphone on (left or right - generally these devices default to left).

```
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// #define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT
```

And you will need to tell it which pins you have connected to the microphone:


#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_32
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_33
```

If you want to have speaker output then you will need to connect or change the following pins to your I2S amplifier:

```
// speaker settings
#define I2S_SPEAKER_SERIAL_CLOCK GPIO_NUM_14
#define I2S_SPEAKER_LEFT_RIGHT_CLOCK GPIO_NUM_12
#define I2S_SPEAKER_SERIAL_DATA GPIO_NUM_27
```
In my Thesis project I don't use speaker.

IF you want to use led to perform status of Marvin. You can specify Pin out and define own change in Indicator.cpp


Finally, we have the access key for wit.ai - I will leave my key active for as long as possible.

```
// command recognition settings
#define COMMAND_RECOGNITION_ACCESS_KEY "UFFEIMRQL7LH4T2DXRHKER4HPMFB4LNH"
```


In my project, I Want connect to wifi instead to define in config.h, I use Wifi Access Point to set up and connect, finally save them into the EEPROOM.

And using firebase to share data with my App and another the ESP in my house.

CODING FLOW 
Firstly, I config I2S pin for MIC and SPEAKER (optional).
--Starting Mic's thread for always collectiong and saving them into circular buffer and ping to APP's thread to run Detection application. In Detectly mode,
Certainly receive Marvin's word and move to Recognization mode. In this mode we need provide some command we setup before in Wit.ai to 