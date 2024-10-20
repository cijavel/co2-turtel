#ifndef CO2_TURTLE_WEBSERVERHANDLER_H
#define CO2_TURTLE_WEBSERVERHANDLER_H

#include "ESPAsyncWebServer.h"
#include "DataCO2.h"
#include "bsec.h"
#include "FS.h"
#include "LittleFS.h"

class WebServerHandler {
public:
    static WebServerHandler &getInstance() {
        static WebServerHandler instance; // Guaranteed to be destroyed.
        return instance;// Instantiated on first use.
    }
    void start();
    void setInputDataforBody(DataCO2 co2Sensordata, Bsec enviromentdata, String sdate);
private:
    DataCO2 co2data;
    Bsec bmedata;
    AsyncWebServer server = AsyncWebServer(80);
    String acDate;
    static void handle_index(AsyncWebServerRequest *request);
    void handle_data(AsyncWebServerRequest *request);
    void handle_status(AsyncWebServerRequest *request);
    void handle_wlan(AsyncWebServerRequest *request);
    void handle_setting(AsyncWebServerRequest *request);
    void handle_setting_submit(AsyncWebServerRequest *request);
    void handle_switch_submit(AsyncWebServerRequest *request);
    void handle_credentials_submit(AsyncWebServerRequest *request);
    void handle_reset(AsyncWebServerRequest *request);
    static void handle_NotFound(AsyncWebServerRequest *request);
    WebServerHandler();                    // Constructor? (the {} brackets) are needed here.
    WebServerHandler(WebServerHandler const&);  // Don't Implement
    void operator=(WebServerHandler const&); // Don't implement
};
#endif //CO2_TURTLE_WEBSERVERHANDLER_H
