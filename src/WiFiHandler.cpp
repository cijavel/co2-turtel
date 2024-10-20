#include <WiFi.h>
#include "WiFiHandler.h"
// please rename credentials_example.h to credentials.h and set your WIFI Credentials there
#include <Preferences.h>
#include "Credentials.h"
#include "Configuration.h"
#include "WebServerHandler.h"
extern Preferences preferences;
String password;
String ssid;

void WiFiHandler::loadWiFiCredentials()
{
	preferences.begin("config", true); // Read-only mode
	if (preferences.isKey("wlanSSID"))
	{
		ssid = preferences.getString("wlanSSID", WIFI_SSID);
	}
	else
	{
		ssid = WIFI_SSID;
	}
	if (preferences.isKey("wlanPASSWORD"))
	{
		password = preferences.getString("wlanPASSWORD", WIFI_PW);
	}
	else
	{
		password = WIFI_PW;
	}
	preferences.end();
	Serial.print("[WIFI] SSID: ");
	Serial.println(ssid);
}

void WiFiHandler::setupAPMode()
{
	const char *apSSID = "sensorturle 192.168.4.1";
	const char *apPassword = "sensorturtle";
	WiFi.softAP(apSSID, apPassword);
	Serial.println("[AP MODE] Access Point Started");
	Serial.print("[AP MODE] IP address: ");
	Serial.println(WiFi.softAPIP());
}

void WiFiHandler::initWifi()
{
	int wifiWaitCount = 0;
	WiFiClass::setHostname(DeviceName);

	preferences.begin("config", true);
	bool switchDebug = preferences.getBool("switchDEBUG", DEBUG);
	preferences.end();

	if (switchDebug)
	{
		Serial.print("\n[WIFI] Connecting to ");
		Serial.println(WIFI_SSID);
	}

	loadWiFiCredentials();
	WiFi.begin(ssid.c_str(), password.c_str());

	while (WiFiClass::status() != WL_CONNECTED && wifiWaitCount < 20)
	{
		delay(250);
		wifiWaitCount++;
	}
	if (WiFiClass::status() == WL_CONNECTED)
	{
		Serial.print("[WIFI] IP address: ");
		Serial.println(WiFi.localIP());
	}
	else
	{
		Serial.println("[WIFI] Starting AP mode. Please connect to the esp32 wlan");
		setupAPMode();
	}
}

void WiFiHandler::ReStart()
{
	preferences.begin("config", true);
	bool switchDebug = preferences.getBool("switchDEBUG", DEBUG);
	preferences.end();

	if (switchDebug)
	{
		Serial.println();
		Serial.print("[WIFI] Connecting to ");
		Serial.println(WIFI_SSID);
	}
	loadWiFiCredentials();
	WiFi.begin(ssid.c_str(), password.c_str());
	int wifiWaitCount = 0;
	while (WiFiClass::status() != WL_CONNECTED && wifiWaitCount < 20)
	{
		delay(250);
		wifiWaitCount++;
	}

	if (switchDebug)
	{
		if (WiFiClass::status() == WL_CONNECTED)
		{
			Serial.println();
			Serial.println("[WIFI] WiFi connected");
			Serial.println(WiFi.localIP());
		}
	}
}

bool WiFiHandler::StatusCheck()
{
	preferences.begin("config", true);
	bool switchDebug = preferences.getBool("switchDEBUG", DEBUG);
	preferences.end();

	wl_status_t status = WiFiClass::status();
	if (status != WL_CONNECTED)
	{
		ReStart();
	}
	else
	{
		if (switchDebug)
		{
			Serial.println("[WIFI] Still connected");
			Serial.println();
		}
	}

	return status == WL_CONNECTED;
}

bool WiFiHandler::checkWifi(unsigned long currentSeconds)
{
	if (currentSeconds % interval_WiFiCheck_in_Seconds == 0)
	{
		return WiFiHandler::StatusCheck();
	}
	return false;
}
