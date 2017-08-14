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

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Code

#define DefaultHostName "esp8266fs"

const char* DNSSDTXTDeviceStateKey = "DvSt";
const char* DNSSDTXTDeviceStateSetup = "0";
const char* DNSSDTXTDeviceStateConfigured = "1";

void handleAccessPoints();
void handleConfigure();

void storeDeviceConfiguration(const char* name, const char* ssid, const char* password);


const byte DefaultWebServerPort = 80;

ESP8266WebServer server(DefaultWebServerPort);

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
    
    Serial.println("");
    Serial.println("handleConfigure");
    
    if (server.hasArg("ssid") && server.hasArg("name")) {
        
        String ssid = server.arg("ssid");
        String pass = server.arg("password");
        String name = server.arg("name");
        
        Serial.println(name);
        Serial.println(ssid);
        Serial.println(pass);
        
        bool connected = connectToWIFI(ssid.c_str(), pass.c_str());
        
        if (connected) {
            storeDeviceConfiguration(name.c_str(), ssid.c_str(), "");//pass.c_str());
            setupMDNS(name.c_str(), DNSSDTXTDeviceStateConfigured);
        }
        
        String res = connected ? "1" : "0";
        server.send(200, "text/plain", "{\"success\": " + res + "}");
        
        Serial.println("handleConfigure 200 sended: " + res);
    } else {
        server.send(400, "text/plain", "{\"error\": {\"code\": 5}}");
        
        Serial.println("handleConfigure 500 sended");
    }
}

void handleClient() {
    server.handleClient();
}

void setupMDNS(const char* name, const char* state) {
    
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
    MDNS.addServiceTxt("esp", "tcp", DNSSDTXTDeviceStateKey, state);
}

void storeDeviceConfiguration(const char* name, const char* ssid, const char* password) {
    Serial.println("Storing Device Configuration");
    
    UserConfiguration conf;
    conf.resetConfig();
    conf.setServerName(name);
    conf.setSSID(ssid);
    conf.setPassword(password);
    
    conf.saveConfig();
    Serial.println("Device configuration stored successfully");
}
