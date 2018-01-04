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

#include "ConnectionManager.h"
#include "WebServerManager.h"
#include "UserConfiguration.h"
#include "Led.h"
#include "DeviceService.h"

#include <ESP8266mDNS.h>
#include "FS.h"
#include "Types.h"

#define DefaultAccessPointSSID "ESP8266"

#define EMPTY_PASSWORD NO

const char* ssid = "ABCDk";
const char* password = "13128800'";

WebServerManager currentWebServerManager;

// Functions
void restoreWIFIConnection();

// Public variables
DNSSDTXTDeviceState deviceState;

void setup() {
    
    Serial.begin(115200);
    Serial.println("");
//    Serial.setDebugOutput(true);
    
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }

    DefaultLed.setupLed();
    
    //WIFI INIT
    restoreWIFIConnection();

    currentWebServerManager.connectHandler([](const char* ssid, const char* password) -> bool {

        bool connected = connectToWIFI(ssid, password);

        if (connected) {
            deviceState = DNSSDTXTDeviceState::Configured;
        }
        return connected;
    });

    currentWebServerManager.currentStateHandler([]() -> DNSSDTXTDeviceState {
        return deviceState;
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

void loop() {
    //MDNS.update();
    currentWebServerManager.handleClient();
}
