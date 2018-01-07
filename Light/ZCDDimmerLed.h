///
/// @file		ZCDDimmerLed.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project Light
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Ruslan Maslouski
/// @author		Ruslan Maslouski
///
/// @date		1/6/18 16:24
/// @version	<#version#>
///
/// @copyright	(c) Ruslan Maslouski, 2018
/// @copyright	<#licence#>
///
/// @see		ReadMe.txt for references
///


// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#include "libpandora_types.h"
#include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#include "Arduino.h"
#elif defined(SPARK) // Spark specific
#include "application.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#ifndef ZCDDimmerLed____FILEEXTENSION___
#define ZCDDimmerLed____FILEEXTENSION___

typedef uint8_t DimmerValue;

// Dimmer Led with Zero Cross Detector
class ZCDDimmerLed {

public:
    ZCDDimmerLed(int ledGPIO, int zcdGPIO);

    void visibleLed(DimmerValue value);
    void process();

    DimmerValue getValue();

protected:
    void setupLed();
    void runAnimationStep();
private:
    DimmerValue _currentValue;
    DimmerValue _value;
    int _ledGPIO;
    int _zcdGPIO;

    unsigned long  _stepInterval;
    unsigned long  _lastStepTime;
};

#endif // ZCDDimmerLed____FILEEXTENSION___
