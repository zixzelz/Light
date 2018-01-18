//
// TouchSensorPanel.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 1/11/18 00:00
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2018
// Licence		<#licence#>
//
// See 			TouchSensorPanel.h and ReadMe.txt for references
//


// Library header
#include "TouchSensorPanel.h"
#include "RunLoop.h"

// Code

const int TouchSample = 17;
const int TouchSensitivity = 200;
const int LegacyTouchSensitivity = 205;

TouchSensorPanel::TouchSensorPanel(uint8_t sendGPIO, uint8_t touchGPIO_0, uint8_t touchGPIO_1, uint8_t touchGPIO_2, bool screduled):
TouchSensorPanel(sendGPIO, touchGPIO_0, touchGPIO_1, touchGPIO_2) {

    if (screduled) {
        MainRunLoop.addRepeatBlock([this]() {
            process(); //.cpp:92 __throw_bad_function_call 🤔
        });
    }
}

TouchSensorPanel::TouchSensorPanel(uint8_t sendGPIO, uint8_t touchGPIO_0, uint8_t touchGPIO_1, uint8_t touchGPIO_2):
_sensor0(sendGPIO, touchGPIO_0), _sensor1(sendGPIO, touchGPIO_1), _sensor2(sendGPIO, touchGPIO_2) {

    _sendGPIO = sendGPIO;
    _touchGPIO_0 = touchGPIO_0;
    _touchGPIO_1 = touchGPIO_1;
    _touchGPIO_2 = touchGPIO_2;

    _sensorState_0 = false;
    _sensorState_1 = false;
    _sensorState_2 = false;
}

void TouchSensorPanel::setTouchDownEvent(TouchSensor touchId, TouchHandlerFunction handler) {
    _touchDownHandler[touchId] = handler;
}

void TouchSensorPanel::setTouchUpEvent(TouchSensor touchId, TouchHandlerFunction handler) {
    _touchUpHandler[touchId] = handler;
}

void TouchSensorPanel::process() {
    long start = millis();
    long total0 = _sensor0.capacitiveSensor(TouchSample);
    long total1 = _sensor1.capacitiveSensor(TouchSample);
    long total2 = _sensor2.capacitiveSensor(TouchSample);

//    bool newValue_0, newValue_1, newValue_2;
//
//    if (total0 > TouchSensitivity && (total0 > total1 || total1 > LegacyTouchSensitivity) && total0 > total2 || total2 > LegacyTouchSensitivity) {
//        newValue_0 = true;
//    } else {
//        newValue_0 = false;
//    }
//    if (total1 > TouchSensitivity && (total1 > total0 || total0 > LegacyTouchSensitivity) && total1 > total2 || total2 > LegacyTouchSensitivity) {
//        newValue_1 = true;
//    } else {
//        newValue_1 = false;
//    }
//    if (total2 > TouchSensitivity && (total2 > total0 || total0 > LegacyTouchSensitivity) && total2 > total1 || total1 > LegacyTouchSensitivity) {
//        newValue_2 = true;
//    } else {
//        newValue_2 = false;
//    }
//
//    if (newValue_0 != _sensorState_0) {
//        _sensorState_0 = newValue_0;
//        _sensorStateChanged_0(newValue_0);
//    }
//    if (newValue_1 != _sensorState_1) {
//        _sensorState_1 = newValue_1;
//        _sensorStateChanged_1(newValue_1);
//    }
//    if (newValue_2 != _sensorState_2) {
//        _sensorState_2 = newValue_2;
//        _sensorStateChanged_2(newValue_2);
//    }

    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug window spacing

    Serial.print(total0);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total1);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total2);                // print sensor output 3
    Serial.println();
}

void TouchSensorPanel::_sensorStateChanged(int id, bool newValue) {
    Serial.print("Sensor ");
    Serial.print(id);
    Serial.print(": ");
    Serial.println(newValue);
    if (newValue) {
        TouchHandlerFunction handler = _touchDownHandler[id];
        handler();
    } else {
        TouchHandlerFunction handler = _touchUpHandler[id];
        handler();
    }
}
