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

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "UserConfiguration.h"
#include "Led.h"

const byte NumberOfConnectionAttempts = 25;

#define DefaultAccessPointSSID "ESP8266"
const char* DefaultHostName = "esp8266fs";
const byte DefaultWebServerPort = 80;

const char* DNSSDTXTDeviceStateKey = "DvSt";
const char* DNSSDTXTDeviceStateSetup = "0";
const char* DNSSDTXTDeviceStateConfigured = "1";

const char* ssid = "ABCDk";
const char* password = "13128800'";

// Functions
void restoreWIFIConnection();
bool setupAccessPoint(const char* ssid);
void setupWebServer();
void setupMDNS(const char* state);
void handleAccessPoints();
void handleConfigure();
void storeDeviceConfiguration(const char* name, const char* ssid, const char* password);
bool connectToWIFI(const char* ssid, const char* password);


ESP8266WebServer server(DefaultWebServerPort);

void setup() {
    
    Serial.begin(115200);
    Serial.println("");
    Serial.setDebugOutput(true);
    
    // prepare LedGPIO
    setupLed();
    
    //WIFI INIT
    restoreWIFIConnection();
}

void restoreWIFIConnection() {
    
    WiFi.disconnect(true);
    
    bool connected = false;
    
    UserConfiguration conf;
    
    bool isLoadConfig = conf.loadConfig();
    if (isLoadConfig) {
        Serial.println("Config loaded successful");
        connected = connectToWIFI(ssid, password);
    }
    
    visibleLed(connected);
    
    if (!connected) {
        const char* name = isLoadConfig ? conf.serverName() : 0;
        setupAccessPoint(name ? : DefaultAccessPointSSID);
    }
    
    setupWebServer();
    
    if (connected) {
        setupMDNS(DNSSDTXTDeviceStateConfigured);
    } else {
        //setupMDNS(DNSSDTXTDeviceStateSetup);
    }
}

bool setupAccessPoint(const char* ssid) {
    
    Serial.println("Configuring access point...");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid);
    
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

void setupWebServer() {
    
    server.on("/accessPoints", HTTP_GET, handleAccessPoints);
    server.on("/configure", HTTP_GET, handleConfigure);
    
    server.on("/pin", HTTP_GET, []() {
        Serial.println("pin");
        server.send(200, "text/plain", "pin");
    });
    
    server.onNotFound([]() {
        server.send(404, "text/plain", "FileNotFound");
    });
    
    server.begin();
    Serial.println("HTTP server started");
}

void setupMDNS(const char* state) {
    
    Serial.println("Starting mDNS");
    
    while (!MDNS.begin(DefaultHostName, WiFi.softAPIP())) {
        Serial.println("Error setting up MDNS responder!");
        delay(1000);
    }
    
    Serial.println("mDNS responder started");
    MDNS.addService("esp", "tcp", DefaultWebServerPort);
    MDNS.addServiceTxt("esp", "tcp", "_d", DefaultHostName);
    MDNS.addServiceTxt("esp", "tcp", DNSSDTXTDeviceStateKey, state);
}

void handleAccessPoints() {
    
    //TKIP (WPA) = 2
    //WEP = 5
    //CCMP (WPA) = 4
    //NONE = 7
    //AUTO = 8
    Serial.println("handleAccessPoints");
    
    int n = WiFi.scanNetworks();
    
    String output = "[";
    for (int i = 0; i < n; i++) {
        
        if (output != "[") output += ',';
        output += "{\"ssid\":\"";
        output += WiFi.SSID(i);
        output += "\",\"rssi\":";
        output += String(WiFi.RSSI(i));
        output += ",\"lock\":";
        output += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "0" : "1";
        output += "}";
    }
    
    output += "]";
    server.send(200, "text/json", output);
}

void handleConfigure() {
    
    Serial.println("handleConfigure");
    
    if (server.hasArg("ssid") && server.hasArg("name")) {
        
        String ssid = server.arg("ssid");
        String pass = server.arg("password");
        String name = server.arg("name");
        
        
        
        bool connected = connectToWIFI(ssid.c_str(), pass.c_str());
        
        if (connected) {
            //setupMDNS(DNSSDTXTDeviceStateConfigured);
            storeDeviceConfiguration(name.c_str(), ssid.c_str(), pass.c_str());
        }
        
        String res = connected ? "1" : "0";
        server.send(200, "text/plain", "{\"success\": " + res + "}");
    } else {
        server.send(500, "text/plain", "{\"error\": \"BAD params\"}");
    }
}

void storeDeviceConfiguration(const char* name, const char* ssid, const char* password) {
    
    UserConfiguration conf;
    conf.resetConfig();
    conf.setServerName(name);
    conf.setSSID(ssid);
    conf.setPassword(password);
    
    conf.saveConfig();
}

bool connectToWIFI(const char* ssid, const char* password) {
    
    Serial.printf("Connecting to %s\n", ssid);
    if (String(WiFi.SSID()) != String(ssid)) {
        WiFi.begin(ssid, password);
    }
    
    int step = 0;
    bool result = (WiFi.status() == WL_CONNECTED);
    
    while (!result && step < NumberOfConnectionAttempts) {
        
        blinkLed();
        Serial.print(".");
        
        delay(400);
        step++;
        result = (WiFi.status() == WL_CONNECTED);
    }
    
    if (result) {
        //WiFi.mode(WIFI_OFF);
        
        Serial.println("");
        Serial.print("Connected! IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        WiFi.disconnect();
        Serial.println("Failed connect to AP!");
    }
    
    return result;
}

void loop() {
    //MDNS.update();
    server.handleClient();
}

