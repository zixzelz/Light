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
//
// See 			ZCDDimmerLed.h and ReadMe.txt for references
//


// Library header
#include "ZCDDimmerLed.h"

// Code

#define DimmerMaxValue DimmerLedOn

uint8_t _pin;
unsigned int _zcd_newValue[16];
unsigned int _zcd_currentValue[16];
bool _inProgress[16];
unsigned int _zcd_step;
bool _timerOn = false;

void _timerIsr() {

    for (int pin = 0; pin < 16; pin++) {
        if (_zcd_newValue[pin] != 0 || _zcd_currentValue[pin] != 0) {
            if (_zcd_step == _zcd_currentValue[pin]) {
                digitalWrite(pin, LOW);
            } else if (_zcd_step == 0) {
                digitalWrite(pin, HIGH);
            } else if (_zcd_step >= DimmerMaxValue) {
                _zcd_currentValue[pin] = _zcd_newValue[pin];
            }
        }
    }
    if (_zcd_step >= DimmerMaxValue) {
        _zcd_step = 0;
    } else {
        _zcd_step += 1;
    }
}

void _handleZeroCross() {

    _zcd_step = 0;
    for (int pin = 0; pin < 16; pin++) {
        _zcd_currentValue[pin] = _zcd_newValue[pin];
    }
}

void _startTimer() {
    unsigned int frequency = 10000;//12750;

    timer1_disable();
    timer1_isr_init();
    timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
    timer1_write((clockCyclesPerMicrosecond() * 1000000) / frequency);
    timer1_attachInterrupt(_timerIsr);
}

void setupZeroInterrupt(uint8_t pin) {
    static bool setupd = false;
    if (!setupd) {
        setupd = true;
        attachInterrupt(pin, _handleZeroCross, RISING);
    }
}

void pwmWrite(uint8_t pin, int value) {
    _zcd_newValue[pin] = value;

    if (!_timerOn) {
        _timerOn = true;

        _startTimer();
    }
}

ZCDDimmerLed::ZCDDimmerLed(int ledGPIO) {

    _ledGPIO = ledGPIO;

    _stepInterval = 2;
    setupLed();

    _zcd_step = 0;
}

void ZCDDimmerLed::setupZCD(int zcdGPIO) {
    setupZeroInterrupt(zcdGPIO);
}

void ZCDDimmerLed::setupLed() {
    pinMode(_ledGPIO, OUTPUT);
    digitalWrite(_ledGPIO, LOW);
}

void ZCDDimmerLed::visibleLed(DimmerValue value) {
    Serial.print(_ledGPIO);
    Serial.print(" ");
    Serial.println(value);

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
        pwmWrite(_ledGPIO, _currentValue);
        _lastStepTime = time;
    }
}
