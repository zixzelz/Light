///
/// @file		TouchSensorPanel.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project Light
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Ruslan Maslouski
/// @author		Ruslan Maslouski
///
/// @date		1/11/18 00:00
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

#ifndef TouchSensorPanel____FILEEXTENSION___
#define TouchSensorPanel____FILEEXTENSION___

#include <functional>
#include "CapacitiveSensor.h"
#include "RunLoop.h"

struct _TouchSensor {
    enum Local: char {Sensor_0 = 0, Sensor_1 = 1, Sensor_2 = 2};
};
typedef _TouchSensor::Local TouchSensor;

class TouchSensorPanel {

    public:
    TouchSensorPanel(uint8_t sendGPIO, uint8_t touchGPIO_0, uint8_t touchGPIO_1, uint8_t touchGPIO_2);

    static TouchSensorPanel scheduledTouchSensorPanel(uint8_t sendGPIO, uint8_t touchGPIO_0, uint8_t touchGPIO_1, uint8_t touchGPIO_2) {
        TouchSensorPanel panel = TouchSensorPanel(sendGPIO, touchGPIO_0, touchGPIO_1, touchGPIO_2);

//        MainRunLoop.addRepeatBlock([]() {
            //panel.process();
//        });
        return panel;
    }

    typedef std::function<void(void)> TouchHandlerFunction;
    void setTouchDownEvent(TouchSensor touchId, TouchHandlerFunction handler);
    void setTouchUpEvent(TouchSensor touchId, TouchHandlerFunction handler);

    void process();

    private:
    uint8_t _sendGPIO;
    uint8_t _touchGPIO_0;
    uint8_t _touchGPIO_1;
    uint8_t _touchGPIO_2;

    bool _sensorState_0;
    bool _sensorState_1;
    bool _sensorState_2;

    CapacitiveSensor _sensor0;
    CapacitiveSensor _sensor1;
    CapacitiveSensor _sensor2;

    TouchHandlerFunction _touchDownHandler[3];
    TouchHandlerFunction _touchUpHandler[3];

    void _sensorStateChanged(int id, bool newValue);
};

#endif // TouchSensorPanel____FILEEXTENSION___
