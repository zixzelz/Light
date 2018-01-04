///
/// @file		ServerManager.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project Light
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Ruslan Maslouski
/// @author		Ruslan Maslouski
///
/// @date		8/14/17 22:56
/// @version	<#version#>
///
/// @copyright	(c) Ruslan Maslouski, 2017
/// @copyright	<#licence#>
///
/// @see		ReadMe.txt for references
///


// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#include "libpandora_types.h"
#include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#include "Arduino.h"
#elif defined(SPARK) // Spark specific
#include "application.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#ifndef ServerManager_cpp
#define ServerManager_cpp

#include <functional>
#include <ESP8266WebServer.h>
#include "Types.h"

//enum SeekMode123 {
//    SeekSet = 0,
//    SeekCur = 1,
//    SeekEnd = 2
//};

class WebServerManager {
public:
    WebServerManager();

    void setupWebServer();
    void handleClient();
    void setupMDNS(const char* name);

    typedef std::function<bool(const char* ssid, const char* password)> TConnectHandlerFunction;
    void connectHandler(TConnectHandlerFunction handler);

    typedef std::function<DNSSDTXTDeviceState(void)> TCurrentStateHandlerFunction;
    void currentStateHandler(TCurrentStateHandlerFunction handler);

protected:
    ESP8266WebServer _server;

    void _handleState();
    void _handleAccessPoints();
    void _handleConfigure();
    void storeDeviceConfiguration(const char* name, const char* ssid, const char* password);

    TConnectHandlerFunction _connectHandler;
    TCurrentStateHandlerFunction _currentStateHandler;
};

#endif // ServerManager_cpp
