//
// Light
//
// Light
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		Ruslan Maslouski
// 				Ruslan Maslouski
//
// Date			8/10/17 22:06
// Version		<#version#>
//
// Copyright	© Ruslan Maslouski, 2017
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//


// Core library for code-sense - IDE-based
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.8 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#include "RunLoop.h"
#include "ConnectionManager.h"
#include "WebServerManager.h"
#include "UserConfiguration.h"
#include "Led.h"
#include "ZCDDimmerLed.h"
#include "DeviceService.h"
#include "TouchSensorPanel.h"

#include <ESP8266mDNS.h>
#include "FS.h"
#include "Types.h"

#define DefaultAccessPointSSID "ESP8266"

#define EMPTY_PASSWORD NO

const char* ssid = "ABCDk";
const char* password = "13128800'";

WebServerManager currentWebServerManager;
TouchSensorPanel touchSensorPanel(D1, D2, D5, D6);

// Functions
void setupSensorPanel();
bool setLampState(int index, int value);
void restoreWIFIConnection();

// Public variables
DNSSDTXTDeviceState deviceState;
ZCDDimmerLed lampLed_0(D7, D3);
//ZCDDimmerLed lampLed_1(D0, D4);
//ZCDDimmerLed lampLed_2(D0, D7);

void setup() {
    
    Serial.begin(115200);
    Serial.println("");
//    Serial.setDebugOutput(true);
    
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }

    //WIFI INIT
    restoreWIFIConnection();

    currentWebServerManager.connectHandler([](const char* ssid, const char* password) -> bool {

        bool connected = connectToWIFI(ssid, password);

        if (connected) {
            deviceState = DNSSDTXTDeviceState::Configured;
        }
        return connected;
    });

    currentWebServerManager.currentStateHandler([]() -> CurrentState {

        CurrentState state;
        state.deviceState = deviceState;
        state.lampState[0] = lampLed_0.getValue();
        //state.lampState[1] = lampLed_1.getValue();
        //state.lampState[2] = lampLed_2.getValue();

        return state;
    });

    currentWebServerManager.setLampStateHandler([](int index, int value) -> bool {
        return setLampState(index, value);
    });

    MainRunLoop.addRepeatBlock([]() {
        lampLed_0.process();
        //lampLed_1.process();
        //lampLed_2.process();
    });

    setupSensorPanel();
}

void setupSensorPanel() {

    touchSensorPanel.setTouchDownEvent(TouchSensor::Sensor_0, []() {
        int value = lampLed_0.getValue();
        if (value > DimmerLedOff) {
            setLampState(-1, DimmerLedOff);
        } else {
            setLampState(-1, DimmerLedOn);
        }
    });

    touchSensorPanel.setTouchDownEvent(TouchSensor::Sensor_1, []() {

    });

    touchSensorPanel.setTouchDownEvent(TouchSensor::Sensor_2, []() {

    });
}

void restoreWIFIConnection() {
    
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
    
    bool connected = false;
    
    UserConfiguration conf;
    bool isLoadConfig = conf.loadConfig();

    const char* ssid = 0;
    
    const char* _serverName = isLoadConfig ? conf.serverName() : 0;
    String serverName = String(_serverName ?: DefaultAccessPointSSID);
    
    if (isLoadConfig) {
        Serial.println("Config loaded successful");
        Serial.println(conf.serverName());
        Serial.println(conf.ssid());
        Serial.println(conf.password());
        
        ssid = conf.ssid();
    }
    
    if (ssid) {
#if EMPTY_PASSWORD
        connected = connectToWIFI(ssid, "");
#else
        connected = connectToWIFI(ssid, conf.password());
#endif
    }
    deviceState = connected ? DNSSDTXTDeviceState::Configured : DNSSDTXTDeviceState::Setup;

    DefaultLed.visibleLed(connected);
    
    if (!connected) {
        setupAccessPoint(serverName.c_str());
    }
    
    currentWebServerManager.setupWebServer();
    
    if (connected) {
        currentWebServerManager.setupMDNS(serverName.c_str());
    } else {
        currentWebServerManager.setupMDNS(serverName.c_str());
    }
}

bool setLampState(int index, int value) {
    switch (index) {
        case -1: {
            lampLed_0.visibleLed(value);
            //lampLed_1.visibleLed(value);
            //lampLed_2.visibleLed(value);
            break;
        }
        case 0: {
            lampLed_0.visibleLed(value);
            break;
        }
        case 1: {
            //lampLed_1.visibleLed(value);
            break;
        }
        case 2: {
            //lampLed_2.visibleLed(value);
            break;
        }
        default: {
            return false;
        }
    }
    return true;
}

void loop() {
//    Serial.println(_duration);
    //MDNS.update();
    //touchSensorPanel.process();
    currentWebServerManager.handleClient();
    MainRunLoop.process();
}
