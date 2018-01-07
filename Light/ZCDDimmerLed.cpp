//
// ZCDDimmerLed.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 1/6/18 16:24
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2018
// Licence		<#licence#>
//
// See 			ZCDDimmerLed.h and ReadMe.txt for references
//


// Library header
#include "ZCDDimmerLed.h"

// Code

ZCDDimmerLed::ZCDDimmerLed(int ledGPIO, int zcdGPIO) {
    _ledGPIO = ledGPIO;
    _zcdGPIO = zcdGPIO;

    _stepInterval = 2;

    setupLed();
}

void ZCDDimmerLed::setupLed() {
    pinMode(_ledGPIO, OUTPUT);
    digitalWrite(_ledGPIO, LOW);
}

void ZCDDimmerLed::visibleLed(DimmerValue value) {
    _value = value;
    process();
}

DimmerValue ZCDDimmerLed::getValue() {
    return _value;
}

void ZCDDimmerLed::process() {
    if (_currentValue != _value) {
        runAnimationStep();
    }
}

void ZCDDimmerLed::runAnimationStep() {
    unsigned long time = millis();

    if (time > _lastStepTime + _stepInterval) {
        if (_currentValue > _value) {
            _currentValue -= 1;
        } else if (_currentValue < _value) {
            _currentValue += 1;
        }

        analogWrite(_ledGPIO, _currentValue);

        _lastStepTime = time;
    }
}
