//
// ServerManager.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 8/14/17 22:56
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2017
// Licence		<#licence#>
//
// See 			ServerManager.h and ReadMe.txt for references
//


// Library header
#include "WebServerManager.h"
#include "ConnectionManager.h"
#include "UserConfiguration.h"

#include <ESP8266mDNS.h>

// Code

#define DefaultHostName "esp8266fs"

const char* DNSSDTXTDeviceStateKey = "DvSt";

//void storeDeviceConfiguration(const char* name, const char* ssid, const char* password);

const char DefaultWebServerPort = 80;

WebServerManager::WebServerManager() {
    _server = ESP8266WebServer(DefaultWebServerPort);
}

void WebServerManager::setupWebServer() {

    _server.on("/state", HTTP_GET, [this]() {
        _handleState();
    });

    _server.on("/accessPoints", HTTP_GET, [this]() {
        _handleAccessPoints();
    });

    _server.on("/configure", HTTP_GET, [this]() {
        _handleConfigure();
    });

    _server.on("/pin", HTTP_GET, [&]() {
        Serial.println("pin");
        _server.send(200, "text/plain", "pin");
    });
    
    _server.onNotFound([&]() {
        _server.send(404, "text/plain", "FileNotFound");
    });
    
    _server.begin();
    Serial.println("HTTP server started");
}

void WebServerManager::_handleState() {

    Serial.println("handleState");

    DNSSDTXTDeviceState state = _currentStateHandler();
    String strState = String(state);

    _server.send(200, "text/json", "{\"state\": " + strState + "}");
}

void WebServerManager::_handleAccessPoints() {
    
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
    _server.send(200, "text/json", output);
}

void WebServerManager::_handleConfigure() {
    
    Serial.println("");
    Serial.println("handleConfigure");
    
    if (_server.hasArg("ssid") && _server.hasArg("name")) {
        
        String ssid = _server.arg("ssid");
        String pass = _server.arg("password");
        String name = _server.arg("name");
        
        Serial.println(name);
        Serial.println(ssid);
        Serial.println(pass);
        
        bool connected = _connectHandler(ssid.c_str(), pass.c_str());
        
        if (connected) {
            storeDeviceConfiguration(name.c_str(), ssid.c_str(), pass.c_str());
//            setupMDNS(name.c_str(), DNSSDTXTDeviceStateConfigured);
        }
        
        String res = connected ? "1" : "0";
        _server.send(200, "text/plain", "{\"success\": " + res + "}");
        
        Serial.println("handleConfigure 200 sended: " + res);
    } else {
        _server.send(400, "text/plain", "{\"error\": {\"code\": 5}}");
        
        Serial.println("handleConfigure 400 sended");
    }
}

void WebServerManager::handleClient() {
    _server.handleClient();
}

void WebServerManager::connectHandler(TConnectHandlerFunction handler) {
    _connectHandler = handler;
}

void WebServerManager::currentStateHandler(TCurrentStateHandlerFunction handler) {
    _currentStateHandler = handler;
}

void WebServerManager::setupMDNS(const char* name) {

    Serial.print("Starting mDNS with name: ");
    Serial.println(name);

    const char* title = name ? : DefaultHostName;

    while (!MDNS.begin(title, WiFi.softAPIP())) {
        Serial.println("Error setting up MDNS responder!");
        delay(1000);
    }
    
    Serial.println("mDNS responder started");
    
    MDNS.addService("esp", "tcp", DefaultWebServerPort);
    MDNS.addServiceTxt("esp", "tcp", "_d", title);
//    MDNS.addServiceTxt("esp", "tcp", DNSSDTXTDeviceStateKey, state);
}

void WebServerManager::storeDeviceConfiguration(const char* name, const char* ssid, const char* password) {
    Serial.println("Storing Device Configuration");

    UserConfiguration conf;
    conf.resetConfig();
    conf.setServerName(name);
    conf.setSSID(ssid);
    conf.setPassword(password);

    conf.saveConfig();
    Serial.println("Device configuration stored successfully");
}
