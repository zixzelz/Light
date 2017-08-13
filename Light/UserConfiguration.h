///
/// @file		UserConfiguration.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project Light
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Ruslan Maslouski
/// @author		Ruslan Maslouski
///
/// @date		8/10/17 22:20
/// @version	<#version#>
///
/// @copyright	(c) Ruslan Maslouski, 2017
///
/// @see		ReadMe.txt for references
///

#ifndef UserConfiguration_cpp
#define UserConfiguration_cpp

#include <ArduinoJson.h>

class UserConfiguration {
    
private:
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject *jsonObj;
    
public:
    
    bool loadConfig();
    void resetConfig();
    bool saveConfig();
    
    void setServerName(const char* serverName);
    const char* serverName();
    
    void setSSID(const char* ssid);
    const char* ssid();
    
    void setPassword(const char* password);
    const char* password();
    
};

#endif // UserConfiguration_cpp
