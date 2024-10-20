#ifndef CO2_TURTLE_CONFIGURATION_H
#define CO2_TURTLE_CONFIGURATION_H

//uncomment to get debug in serial
//#define DEBUG 1

//uncomment to switch off
#define switch_WIFI true
#define switch_WEB true
#define switch_PRINT false
#define switch_EPD true
#define switch_LED true
#define switch_MQTT false

#define interval_MHZ19_in_Seconds 30
#define interval_BME680_in_Seconds 30
#define interval_WiFiCheck_in_Seconds 300
#define interval_RAMPrintout_in_Seconds 600
#define interval_EPD_in_Seconds 900
#define interval_LED_in_Seconds 30
#define interval_mqtt_in_Seconds 30

#define PIN_BME680_SDA 21
#define PIN_BME680_SCL 22
#define PIN_MHZ19_RX 17
#define PIN_MHZ19_TX 16
#define BAUDRATE 9600

#define DeviceName "Sensorturtle_3 15.10.2024"
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

#define SEALEVELPRESSURE_HPA 1015
#define TEMPERATUR_OFFSET -3.5
#define BRIGHTNESS_LEDS 2 // dont set 1. it will all leds set to red
#define NUM_LEDS 34
#define DATA_LED_PIN 4

#define MAX_POWER_MILLIAMPS 500
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define MQTT_HOST IPAddress(192, 168, 178, 38)
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883
#define MQTT_USER_ENABLED 1

#endif //CO2_TURTLE_CONFIGURATION_H