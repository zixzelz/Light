//
// UserConfiguration.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 8/10/17 22:20
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2017
// Licence		<#licence#>
//
// See 			UserConfiguration.h and ReadMe.txt for references
//


// Library header
#include "UserConfiguration.h"
#include "UserConfiguration.h"
#include "FS.h"

// Constants

#define ConfigFile "/config.json"
#define ServerNameKey "name"
#define SSIDKey "ssid"
#define PasswordKey "password"

// Code

bool UserConfiguration::loadConfig() {
    File configFile = SPIFFS.open(ConfigFile, "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        return false;
    }
    
    size_t size = configFile.size();
    if (size > 1024) {
        Serial.println("Config file size is too large");
        return false;
    }
    
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);
    
    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);
    
    JsonObject& json = jsonBuffer.parseObject(buf.get());
    
    if (!json.success()) {
        Serial.println("Failed to parse config file");
        return false;
    }
    
    jsonObj = &json;
    return true;
}

void UserConfiguration::resetConfig() {
    JsonObject& json = jsonBuffer.createObject();
    jsonObj = &json;

    json["KEY"] = "";
}

bool UserConfiguration::saveConfig() {
    File configFile = SPIFFS.open(ConfigFile, "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return false;
    }
    
    JsonObject& json = *jsonObj;
    json.printTo(configFile);
    return true;
}

void UserConfiguration::setServerName(const char* serverName) {
    JsonObject& json = *jsonObj;
    json[ServerNameKey] = serverName;
}

const char* UserConfiguration::serverName() {
    JsonObject& json = *jsonObj;
    return json[ServerNameKey];
}

void UserConfiguration::setSSID(const char* ssid) {
    JsonObject& json = *jsonObj;
    json[SSIDKey] = ssid;
}

const char* UserConfiguration::ssid() {
    JsonObject& json = *jsonObj;
    return json[SSIDKey];
}

void UserConfiguration::setPassword(const char* password) {
    JsonObject& json = *jsonObj;
    json[PasswordKey] = password;
}

const char* UserConfiguration::password() {
    JsonObject& json = *jsonObj;
    return json[PasswordKey];
}
