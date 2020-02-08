# Hygrometer thermometer Igormeter

## Components:
Arduino 
LCD Screen
Buzzer
DHT11 or DHT22 Sensor
10k ohm Potentiometer
220 ohm Resistor
100 ohm Resistor
IReciver
IRemote
Breadbord
Cables


## Features
Monitoring temperature and humidity

There are two modes:
- showOnly (default)
- humidityAlarm (default 60%)

The showOnly mode consists of only showing the value of temperature and humidity
The humidityAlarm mode is also emitting a sound if the humidity value is over the threshold.

Press 1 to use it in showOnly mode
Press 2 to use it in humidityAlarm mode
Press FUNC and << or >> to change the threshold
Press 0 for the credits


## Arduino Libraries conflicts
There is a conflict between the IRemote Library and the tone library 
that was cousing the following error:
Tone.cpp.o (symbol from plugin): In function `timer0_pin_port':
(.text+0x0): multiple definition of `__vector_7'
libraries/IRremote/IRremote.cpp.o (symbol from plugin):(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
exit status 1
Error compiling for board Arduino Uno.

To solve this it is necessary to change a timer pin value in the IRemote Library.
Go to Arduino -> Library -> IRemote folder
edit boarddefs.h (or in the old versions IRemoteinit.h)
and uncomment (for Arduino Uno in this case) the IR_USE_TIMER2.

From
```
...
#else
// Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
// ATmega48, ATmega88, ATmega168, ATmega328
	//#define IR_USE_TIMER1   // tx = pin 9
	#define IR_USE_TIMER2     // tx = pin 3
...
```

to this

```
...
#else
// Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
// ATmega48, ATmega88, ATmega168, ATmega328
	#define IR_USE_TIMER1   // tx = pin 9
	//#define IR_USE_TIMER2     //tx = pin 3
...
```

This should do the magic
