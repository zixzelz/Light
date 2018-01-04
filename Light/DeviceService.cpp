//
//  DeviceService.cpp
//  Index
//
//  Created by Ruslan Maslouski on 1/3/18.
//  Copyright © 2018 Ruslan Maslouski. All rights reserved.
//

#include "DeviceService.h"

extern void didOn();

DeviceService::DeviceService(int devicePin) {
    _devicePin = devicePin;
    pinMode(_devicePin, OUTPUT);
}

void DeviceService::setValue(int value) {
    analogWrite(_devicePin, value);
}

void DeviceService::turnOn() {
    setValue(255);
}

void DeviceService::turnOff() {
    setValue(0);
    didOn();
}
