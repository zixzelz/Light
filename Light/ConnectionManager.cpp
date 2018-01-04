//
// ConnectionManager.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 8/14/17 23:02
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2017
// Licence		<#licence#>
//
// See 			ConnectionManager.h and ReadMe.txt for references
//


// Library header
#include "ConnectionManager.h"
#include "Led.h"

#include "ESP8266WiFi.h"

// Code

const byte NumberOfConnectionAttempts = 25;

bool setupAccessPoint(const char* ssid) {
    
    Serial.print("Configuring access point: ");
    Serial.println(ssid);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid);
    
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

bool connectToWIFI(const char* ssid, const char* password) {
    
    Serial.printf("Connecting to %s\r\n", ssid);
    if (String(WiFi.SSID()) != String(ssid)) {
        WiFi.begin(ssid, password);
    }
    
    int step = 0;
    bool result = (WiFi.status() == WL_CONNECTED);
    
    while (!result && step < NumberOfConnectionAttempts) {

        DefaultLed.blinkLed();
        Serial.print(".");

        delay(400);
        step++;
        result = (WiFi.status() == WL_CONNECTED);
    }
    Serial.println("");
    
    if (result) {
        Serial.print("Connected! IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        WiFi.disconnect();
        Serial.println("Failed connect to AP!\r\n");
    }
    
    return result;
}
