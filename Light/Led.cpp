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

// Code

const byte LedGPIO = 2;

void setupLed() {
    
    pinMode(LedGPIO, OUTPUT);
    digitalWrite(LedGPIO, HIGH);
}

void blinkLed() {
    digitalWrite(LedGPIO, LOW);
    delay(100);
    digitalWrite(LedGPIO, HIGH);
}

void visibleLed(bool value) {
    digitalWrite(LedGPIO, !value);
}
