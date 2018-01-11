//
// Led.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 8/11/17 00:32
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2017
// Licence		<#licence#>
//
// See 			Led.h and ReadMe.txt for references
//

// Library header
#include "Led.h"

LedHelper::LedHelper(int ledGPIO) {
    _ledGPIO = ledGPIO;
    _setupLed();
}

void LedHelper::_setupLed() {
    pinMode(_ledGPIO, OUTPUT);
    digitalWrite(_ledGPIO, HIGH);
}

void LedHelper::blinkLed() {
    digitalWrite(_ledGPIO, LOW);
    delay(100);
    digitalWrite(_ledGPIO, HIGH);
}

void LedHelper::visibleLed(bool value) {
    digitalWrite(_ledGPIO, !value);
}

LedHelper DefaultLed(2);
