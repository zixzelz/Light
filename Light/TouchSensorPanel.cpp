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
const int TouchSensitivity = 150;
const int TouchSensitivityToDisable = 50;
const int LegacyTouchSensitivity = 205;
const int MinTouchMoveDelay = 100;

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

    _sensorMax_0 = _sensorMax_1 = _sensorMax_2 =  0;
}

void TouchSensorPanel::setTouchDownEvent(TouchSensor touchId, TouchHandlerFunction handler) {
    _touchDownHandler[touchId] = handler;
}

void TouchSensorPanel::setTouchUpEvent(TouchSensor touchId, TouchHandlerFunction handler) {
    _touchUpHandler[touchId] = handler;
}

void TouchSensorPanel::setTouchMoveEvent(TouchSensor touchId, TouchHandlerFunction handler) {
    _touchMoveHandler[touchId] = handler;
}

void TouchSensorPanel::process() {
    long start = millis();
    long total0 = _sensor0.capacitiveSensor(TouchSample);
    long total1 = _sensor1.capacitiveSensor(TouchSample);
    long total2 = _sensor2.capacitiveSensor(TouchSample);

    bool newValue_0 = false;
    bool newValue_1 = false;
    bool newValue_2 = false;

//    Serial.print(total0);
//    Serial.print(" ");

    if (total0 > TouchSensitivity || (_sensorState_0 && total0 > TouchSensitivityToDisable)) {
        newValue_0 = true;
    }
    if (total1 > TouchSensitivity || (_sensorState_1 && total1 > TouchSensitivityToDisable)) {
        newValue_1 = true;
    }
    if (total2 > TouchSensitivity || (_sensorState_2 && total2 > TouchSensitivityToDisable)) {
        newValue_2 = true;
    }

    if (newValue_0 != _sensorState_0) {
        Serial.print(" Sensor id: 0, =============== value: ");
        Serial.println(total0);

        _sensorState_0 = newValue_0;
        _sensorStateChanged(TouchSensor::Sensor_0, newValue_0);
    } else if (_sensorState_0) {
//        Serial.print(" Sensor id: 0, move: ");
//        Serial.println(total0);

        _sensorStateMove(TouchSensor::Sensor_0);
    }
    if (newValue_1 != _sensorState_1) {
        //Serial.println(total1);
        _sensorState_1 = newValue_1;
        _sensorStateChanged(TouchSensor::Sensor_1, newValue_1);
    }
    if (newValue_2 != _sensorState_2) {
        //Serial.println(total2);
        _sensorState_2 = newValue_2;
        _sensorStateChanged(TouchSensor::Sensor_2, newValue_2);
    }

    _testMaxMin(total0, total1, total2);
}

void TouchSensorPanel::_sensorStateChanged(int id, bool newValue) {
//    Serial.print(" Sensor id: ");
//    Serial.print(id);
//    Serial.print(", value: ");
//    Serial.println(newValue);
    if (newValue) {
        TouchHandlerFunction handler = _touchDownHandler[id];
        if (handler) {
            handler();
        }
    } else {
        TouchHandlerFunction handler = _touchUpHandler[id];
        if (handler) {
            handler();
        }
    }
}

void TouchSensorPanel::_sensorStateMove(int id) {
//        Serial.print(" Sensor id: ");
//        Serial.print(id);
//        Serial.print(", value: ");
//        Serial.println(newValue);

    unsigned long mill = millis();
    if (mill - _touchMoveDelay[id] > MinTouchMoveDelay) {
        _touchMoveDelay[id] = mill;

        TouchHandlerFunction handler = _touchMoveHandler[id];
        if (handler) {
            handler();
        }
    }
}

void TouchSensorPanel::_testMaxMin(int total0, int total1, int total2) {

    bool print = false;
    if (total0 > _sensorMax_0) {
        _sensorMax_0 = total0;
        print = true;
    }
    if (total1 > _sensorMax_1) {
        _sensorMax_1 = total1;
        print = true;
    }
    if (total2 > _sensorMax_2) {
        _sensorMax_2 = total2;
        print = true;
    }

    if (print) {
        Serial.print("Sensor 0 max: ");
        Serial.print(_sensorMax_0);

        Serial.print("; 1 max: ");
        Serial.print(_sensorMax_1);

        Serial.print("; 2 max: ");
        Serial.println(_sensorMax_2);
    }
}
