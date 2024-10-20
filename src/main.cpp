#include <Arduino.h>
// _______________
// EPD ePaper eINK
// ---------------
// BUSY 	  -> -1			Violett,
// RST 	  	  -> -1 		RX2	Blau,
// DC 		  -> 27  		TX2	grün,
// CS 		  -> SS(5)		gelb,
// CLK 		  -> SCK(18)	orange,
// DIN /SDI   -> MOSI(23) 	weiß,
// GND 		  -> GND,
// 3.3V 	  -> 3.3V
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_Z98c // GDEY0213Z98 122x250, SSD1680


// _______________
// LED
// ---------------
// DATA -> 4

// _______________
// MH-Z19B
// ---------------
// GND -> GND
// VCC -> 5V
// RX  -> 17
// TX  -> 16

// _______________
// Bosch BME680
// ---------------
// GND -> GND
// VCC -> 5V
// SCL -> 22
// SDA -> 21
// SD0 -> -
// CS  -> -

//OOP
#include "BME680Handler.h"
#include "MHZ19Handler.h"
#include "Configuration.h"
#include "Credentials.h"
#include "WiFiHandler.h"
#include "bsec.h"
#include <ctime>
#include "EPDHandler.h"
#include "WebServerHandler.h"
#include "FastLedHandler.h"
#include "MqttClientHandler.h"
#include "SettingsHandler.h"
#include <Preferences.h>
Preferences preferences;

// --------------------------------------------------------------------------
// time functions
// --------------------------------------------------------------------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

String localTime(const String& format) {
    struct tm timeinfo{};

    String time = "";
    char toutp[60];
    setenv("TZ", TIMEZONE , 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
    tzset();

    if (!getLocalTime(&timeinfo)) {
        time = "TIME: Failed to obtain";
    } else {
        strftime(toutp, sizeof(toutp), format.c_str(), &timeinfo);
        time = String(toutp);
    }
    return time;
}



static void PrintRamUsage(unsigned long currentSeconds) {
    preferences.begin("config", true);
        bool switchDebug = preferences.getBool("switchDEBUG", DEBUG);
    preferences.end();

    if (switchDebug) {
        if (currentSeconds % interval_RAMPrintout_in_Seconds == 0) {
            Serial.print("Memory Usage: ");
            uint32_t freeHeap = ESP.getFreeHeap();
            uint32_t maximumHeap = ESP.getHeapSize();
            uint32_t usedHeap = maximumHeap - freeHeap;
            Serial.print(usedHeap);
            Serial.print("b | ");
            Serial.print(maximumHeap);
            Serial.println("b");
        }
    } 
}


void setup() {
    delay(100);
    Serial.begin(BAUDRATE);
    Serial.println();
    SettingsHandler &settingsHandler = SettingsHandler::getInstance();
    settingsHandler.setSeetingsOnFirstRun();

    
    preferences.begin("config", true);
        bool switchLED = preferences.getBool("switchLED", switch_LED);
        bool switchmqtt = preferences.getBool("switchMQTT", switch_MQTT);
    preferences.end();
   
    WiFiHandler::initWifi();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    WebServerHandler &webServer = WebServerHandler::getInstance();
    webServer.start();

    if (switchLED) {
        FastLedHandler &ledhandler = FastLedHandler::getInstance();
        ledhandler.setup_led();
    }

    if (switchLED) {
        MqttClientHandler &MqttHandler = MqttClientHandler::getInstance();
        MqttHandler.setup_Mqtt();
    }
}

unsigned long last = 0;
void loop() {
    preferences.begin("config", true);
        bool switchPrint = preferences.getBool("switchPRINT", switch_PRINT);
        bool switchEPD = preferences.getBool("switchEPD", switch_EPD);
        bool switchLED = preferences.getBool("switchLED", switch_LED);
        bool switchMQTT = preferences.getBool("switchMQTT", switch_MQTT);
        bool switchWiFi = preferences.getBool("switchWIFI", switch_WIFI);
        bool switchDEBUG = preferences.getBool("switchDEBUG", DEBUG);
    preferences.end();

    unsigned long currentSeconds = millis() / 1000;
    if (switchDEBUG) {
        if (currentSeconds != last) {
            Serial.print("loop second: ");
            Serial.println(currentSeconds);
            last = currentSeconds;
        }
    } 

    BME680Handler &bmehandler = BME680Handler::getInstance();
    Bsec bme_data = bmehandler.getData();
    if (switchDEBUG) {
        if (bmehandler.updateSensorData(currentSeconds)) {
            bmehandler.printout();
        }
    } else {
        bmehandler.updateSensorData(currentSeconds);
    }

    MHZ19Handler &mhz19Handler = MHZ19Handler::getInstance();
    if (switchDEBUG) {
        if (mhz19Handler.runUpdate(currentSeconds)) {
            mhz19Handler.printoutLastReadout();
        }
    } else {
        mhz19Handler.runUpdate(currentSeconds);
    }
    
    DataCO2 mhz19Readout = mhz19Handler.getLastReadout();

    if (switchWiFi) {
        WiFiHandler::checkWifi(currentSeconds);
    } 
    
    WebServerHandler &webServer = WebServerHandler::getInstance();
    webServer.setInputDataforBody(mhz19Readout, bme_data, localTime("%Y.%m.%d %H:%M:%S"));   

    if (switchEPD) {
        EPDHandler::updateEPDvertical(mhz19Readout, bme_data, localTime("%Y.%m.%d"), localTime("%H:%M"), currentSeconds);
    } 
        
    if (switchLED) {
        FastLedHandler &ledHandler = FastLedHandler::getInstance();
        ledHandler.setInputDataforLED(mhz19Readout, bme_data);
        ledHandler.ledstatus(currentSeconds);
    } else {
        FastLedHandler &ledHandler = FastLedHandler::getInstance();
        ledHandler.setup_black(currentSeconds);
    }

    if (switchMQTT) {
        MqttClientHandler &MqttHandler = MqttClientHandler::getInstance();
        MqttHandler.publishData(mhz19Readout, bme_data, currentSeconds);
    } 
    if (switchDEBUG) {
        PrintRamUsage(currentSeconds);
    } 
}