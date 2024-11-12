#include "WebServerHandler.h"
#include "Configuration.h"
#include "Credentials.h"
#include "settingsHandler.h"
SettingsHandler settingsHandler;

extern Preferences preferences;

#include "../html/index.h"
#include "../html/settings.h"
#include "../html/wlan.h"
#include "../html/status.h"
#include "../html/unifiedCSS.h"
#include "../html/layout.h"

WebServerHandler::WebServerHandler()
{
	server.on("/", HTTP_GET, handle_page_index);
	server.on("/json", HTTP_GET, std::bind(&WebServerHandler::handle_page_data, this, std::placeholders::_1));
	server.on("/status", HTTP_GET, std::bind(&WebServerHandler::handle_page_status, this, std::placeholders::_1));
	server.on("/WLAN", HTTP_GET, std::bind(&WebServerHandler::handle_page_wlan, this, std::placeholders::_1));
	server.on("/settings", HTTP_GET, std::bind(&WebServerHandler::handle_page_setting, this, std::placeholders::_1));
	server.on("/submit", HTTP_POST, std::bind(&WebServerHandler::handle_submit_credentials, this, std::placeholders::_1));
	server.on("/submitsetting", HTTP_POST, std::bind(&WebServerHandler::handle_submit_setting, this, std::placeholders::_1));
	server.on("/submitswitch", HTTP_POST, std::bind(&WebServerHandler::handle_submit_switch, this, std::placeholders::_1));
	server.on("/reset", HTTP_POST, std::bind(&WebServerHandler::handle_reset, this, std::placeholders::_1));
	server.on("/load_defaults", HTTP_POST, std::bind(&WebServerHandler::handle_load_defaults, this, std::placeholders::_1));
	server.onNotFound(handle_page_NotFound);
}

void WebServerHandler::handle_page_index(AsyncWebServerRequest *request)
{
	String name = "index";
	String content = html_index;
	String css = html_css;

	content.replace("{{name}}", name);
	content.replace("{{title}}", DeviceName);
	content.replace("{{unifiedCSS}}", css);
	request->send(200, "text/html; charset=utf-8", content);
}

void WebServerHandler::handle_page_data(AsyncWebServerRequest *request)
{
	String header_data = "{\n";
	header_data += "\"bme680/temperature\":\"{{temperature}}\",\n";
	header_data += "\"bme680/temperature_offset\":\"{{temperature_offset}}\",\n";
	header_data += "\"bme680/temperature_raw\":\"{{temperature_raw}}\",\n";
	header_data += "\"bme680/humidity\":\"{{humidity}}\",\n";
	header_data += "\"bme680/humidity_raw\":\"{{humidity_raw}}\",\n";
	header_data += "\"bme680/pressure\":\"{{pressure}}\",\n";
	header_data += "\"bme680/gas\":\"{{gas}}\",\n";
	header_data += "\"bme680/bme68xStatus\":\"{{bme68xStatus}}\",\n";
	header_data += "\"bme680/breathVocAccuracy\":\"{{breathVocAccuracy}}\",\n";
	header_data += "\"bme680/breathVocEquivalent\":\"{{breathVocEquivalent}}\",\n";
	header_data += "\"bme680/bsecStatus\":\"{{bsecStatus}}\",\n";
	header_data += "\"bme680/co2Accuracy\":\"{{co2Accuracy}}\",\n";
	header_data += "\"bme680/co2Equivalent\":\"{{co2Equivalent}}\",\n";
	header_data += "\"bme680/compGasAccuracy\":\"{{compGasAccuracy}}\",\n";
	header_data += "\"bme680/compGasValue\":\"{{compGasValue}}\",\n";
	header_data += "\"bme680/gasPercentage\":\"{{gasPercentage}}\",\n";
	header_data += "\"bme680/gasPercentageAccuracy\":\"{{gasPercentageAccuracy}}\",\n";
	header_data += "\"bme680/iaq\":\"{{iaq}}\",\n";
	header_data += "\"bme680/iaqAccuracy\":\"{{iaqAccuracy}}\",\n";
	header_data += "\"bme680/staticIaqAccuracy\":\"{{staticIaqAccuracy}}\",\n";
	header_data += "\"mhz19/Accuracy\":\"{{mhz19Accuracy}}\",\n";
	header_data += "\"mhz19/Background\":\"{{mhz19Background}}\",\n";
	header_data += "\"mhz19/Limited\":\"{{mhz19Limited}}\",\n";
	header_data += "\"mhz19/Raw\":\"{{mhz19Raw}}\",\n";
	header_data += "\"mhz19/Regular\":\"{{mhz19Regular}}\",\n";
	header_data += "\"mhz19/TempAdjustment\":\"{{mhz19TempAdjustment}}\",\n";
	header_data += "\"mhz19/Temperature\":\"{{mhz19Temperature}}\"\n"; // Last entry, no trailing comma
	header_data += "}";

	// Now use replace() to substitute placeholders with actual sensor data
	header_data.replace("{{temperature}}", String(bmedata.temperature));
	header_data.replace("{{temperature_offset}}", String(bmedata.temperature + TEMPERATUR_OFFSET));
	header_data.replace("{{temperature_raw}}", String(bmedata.rawTemperature));
	header_data.replace("{{humidity}}", String(bmedata.humidity));
	header_data.replace("{{humidity_raw}}", String(bmedata.rawHumidity));
	header_data.replace("{{pressure}}", String(bmedata.pressure));
	header_data.replace("{{gas}}", String(bmedata.gasResistance));
	header_data.replace("{{bme68xStatus}}", String(bmedata.bme68xStatus));
	header_data.replace("{{breathVocAccuracy}}", String(bmedata.breathVocAccuracy));
	header_data.replace("{{breathVocEquivalent}}", String(bmedata.breathVocEquivalent));
	header_data.replace("{{bsecStatus}}", String(bmedata.bsecStatus));
	header_data.replace("{{co2Accuracy}}", String(bmedata.co2Accuracy));
	header_data.replace("{{co2Equivalent}}", String(bmedata.co2Equivalent));
	header_data.replace("{{compGasAccuracy}}", String(bmedata.compGasAccuracy));
	header_data.replace("{{compGasValue}}", String(bmedata.compGasValue));
	header_data.replace("{{gasPercentage}}", String(bmedata.gasPercentage));
	header_data.replace("{{gasPercentageAccuracy}}", String(bmedata.gasPercentageAccuracy));
	header_data.replace("{{iaq}}", String(bmedata.iaq));
	header_data.replace("{{iaqAccuracy}}", String(bmedata.iaqAccuracy));
	header_data.replace("{{staticIaqAccuracy}}", String(bmedata.staticIaqAccuracy));
	header_data.replace("{{mhz19Accuracy}}", String(co2data.getAccuracy()));
	header_data.replace("{{mhz19Background}}", String(co2data.getBackground()));
	header_data.replace("{{mhz19Limited}}", String(co2data.getLimited()));
	header_data.replace("{{mhz19Raw}}", String(co2data.getRaw()));
	header_data.replace("{{mhz19Regular}}", String(co2data.getRegular()));
	header_data.replace("{{mhz19TempAdjustment}}", String(co2data.getTempAdjustment()));
	header_data.replace("{{mhz19Temperature}}", String(co2data.getTemperature()));

	request->send(200, "application/json; charset=utf-8", header_data);
}

void WebServerHandler::handle_page_wlan(AsyncWebServerRequest *request)
{
	String password;
	String ssid;
	preferences.begin("config", true);
	ssid = preferences.getString("wlanSSID", WIFI_SSID);
	password = preferences.getString("wlanPASSWORD", WIFI_PW);
	preferences.end();

	String name = "WLAN settings";
	String layout = html_layout;
	String content = html_wlan;
	String css = html_css;

	content.replace("{{ssid}}", String(ssid));
	content.replace("{{password}}", String(password));
	layout.replace("{{content}}", content);
	layout.replace("{{unifiedCSS}}", css);
	layout.replace("{{name}}", name);
	layout.replace("{{title}}", name);

	request->send(200, "text/html", layout);
}

void WebServerHandler::handle_page_status(AsyncWebServerRequest *request)
{
	String content = html_status;
	String css = html_css;
	String name = "Sensor Data";
	String layout = html_layout;

	layout.replace("{{unifiedCSS}}", css);
	layout.replace("{{name}}", name);
	layout.replace("{{title}}", name);

	content.replace("{{deviceName}}", DeviceName);
	content.replace("{{data_gas}}", String(bmedata.gasPercentage));
	content.replace("{{data_breahtvoc}}", String(bmedata.breathVocEquivalent));
	content.replace("{{data_pressure}}", String(bmedata.pressure));
	content.replace("{{data_timestep}}", String(bmedata.outputTimestamp));
	// header_data.replace("{{data_time}}",data_time);
	content.replace("{{data_zone}}", TIMEZONE);
	content.replace("{{data_time}}", acDate);

	content.replace("{{data_iaqaccuracy}}", String(bmedata.iaqAccuracy));
	switch (bmedata.iaqAccuracy)
	{
	case 0:
	{
		content.replace("{{color_iaqaccuracy}}", "777");
		content.replace("{{descr_iaqaccuracy}}", "Calibration phase. Please wait....");
		break;
	}
	case 1:
	{
		content.replace("{{color_iaqaccuracy}}", "FFFF00");
		content.replace("{{descr_iaqaccuracy}}", "learning");
		break;
	}
	case 2:
	{
		content.replace("{{color_iaqaccuracy}}", "ADFF2F");
		content.replace("{{descr_iaqaccuracy}}", "good");
		break;
	}
	case 3:
	{
		content.replace("{{color_iaqaccuracy}}", "006B3C");
		content.replace("{{descr_iaqaccuracy}}", "good. start saving them.");
		break;
	}
	default:
	{
		content.replace("{{color_iaqaccuracy}}", "FF0000");
		content.replace("{{descr_iaqaccuracy}}", "this should never happen! iaqAccuracy < 0 || iaqAccuracy > 3");
	}
	}

	content.replace("{{data_temp}}", String(bmedata.temperature));
	if (bmedata.temperature)
	{
		if (bmedata.temperature < 16) // too cold
		{
			content.replace("{{color_temp}}", "add8e6"); // LightBlue
			content.replace("{{descr_temp}}", "too cold");
		}
		else if (bmedata.temperature < 18) // cold
		{
			content.replace("{{color_temp}}", "0000ff"); // Blue
			content.replace("{{descr_temp}}", "cold");
		}
		else if (bmedata.temperature < 20) // cool
		{
			content.replace("{{color_temp}}", "2e8b57"); // SeaGreen
			content.replace("{{descr_temp}}", "cool");
		}
		else if (bmedata.temperature < 22) // normal
		{
			content.replace("{{color_temp}}", "00ff00"); // Green
			content.replace("{{descr_temp}}", "normal");
		}
		else if (bmedata.temperature < 24) // cosy
		{
			content.replace("{{color_temp}}", "adff2f"); // GreenYellow
			content.replace("{{descr_temp}}", "cosy");
		}
		else if (bmedata.temperature < 26) // warm
		{
			content.replace("{{color_temp}}", "ffff00"); // Yellow
			content.replace("{{descr_temp}}", "warm");
		}
		else if (bmedata.temperature < 28) // hot
		{
			content.replace("{{color_temp}}", "ffa500"); // orange
			content.replace("{{descr_temp}}", "hot");
		}
		else if (bmedata.temperature > 28) // scalding hot
		{
			content.replace("{{color_temp}}", "ff0000"); // Red
			content.replace("{{descr_temp}}", "scalding hot");
		}
		else
		{
			content.replace("{{color_temp}}", "ff00ff"); // Magenta
			content.replace("{{descr_temp}}", "way too hot");
		}
	}

	content.replace("{{data_relativehumidity}}", String(bmedata.humidity));
	if (bmedata.humidity)
	{
		if (bmedata.humidity < 20) // Far too dry
		{
			content.replace("{{color_relativehumidity}}", "ff0000"); // Red
			content.replace("{{descr_relativehumidity}}", "Far too dry");
		}
		else if (bmedata.humidity < 30) // Too dry
		{
			content.replace("{{color_relativehumidity}}", "ffff00"); // Yellow
			content.replace("{{descr_relativehumidity}}", "too dry");
		}
		else if (bmedata.humidity < 40) // dry
		{
			content.replace("{{color_relativehumidity}}", "adff2f"); // GreenYellow
			content.replace("{{descr_relativehumidity}}", "dry");
		}
		else if (bmedata.humidity < 50) // normal
		{
			content.replace("{{color_relativehumidity}}", "00ff00"); // Green
			content.replace("{{descr_relativehumidity}}", "normal");
		}
		else if (bmedata.humidity < 60) // Slightly moist
		{
			content.replace("{{color_relativehumidity}}", "adff2f"); // YellowGreen
			content.replace("{{descr_relativehumidity}}", "Slightly moist");
		}
		else if (bmedata.humidity < 65) // moist
		{
			content.replace("{{color_relativehumidity}}", "ffa500"); // Orange
			content.replace("{{descr_relativehumidity}}", "moist");
		}
		else if (bmedata.humidity < 70) // very moist
		{
			content.replace("{{color_relativehumidity}}", "ff0000"); // Red
			content.replace("{{descr_relativehumidity}}", "very moist");
		}
		else // wet
		{
			content.replace("{{descr_relativehumidity}}", "wet");
			content.replace("{{color_relativehumidity}}", "ff00ff"); // Magenta
		}
	}

	content.replace("{{data_iaq}}", String(bmedata.iaq));
	if (bmedata.iaq > 0)
	{
		if (bmedata.iaq <= 50) // exellent
		{
			content.replace("{{color_iaq}}", "2e8b57"); // SeaGreen
			content.replace("{{descr_iaq}}", "exellent");
		}
		else if (bmedata.iaq <= 100) // good
		{
			content.replace("{{color_iaq}}", "00ff00"); // Green
			content.replace("{{descr_iaq}}", "good");
		}
		else if (bmedata.iaq <= 150) // lightly polluted. Ventilation suggested.
		{
			content.replace("{{color_iaq}}", "9acd32"); // YellowGreen
			content.replace("{{descr_iaq}}", "lightly polluted. Ventilation suggested.");
		}
		else if (bmedata.iaq <= 200) // moderately polluted. please ventilate.
		{
			content.replace("{{color_iaq}}", "ffff00"); // Yellow
			content.replace("{{descr_iaq}}", "moderately polluted. please ventilate.");
		}
		else if (bmedata.iaq < 250) // heavily polluted. please ventilate.
		{
			content.replace("{{color_iaq}}", "ffa500"); // orange
			content.replace("{{descr_iaq}}", "heavily polluted. please ventilate.");
		}
		else if (bmedata.iaq < 300) // severly polluted. please ventilate urgently.
		{
			content.replace("{{color_iaq}}", "ff0000"); // Red
			content.replace("{{descr_iaq}}", "severly polluted. please ventilate urgently.");
		}
		else // extremly polluted. please ventilate urgently.
		{
			content.replace("{{descr_iaq}", "extremly polluted. please ventilate urgently.");
			content.replace("{{color_iaq}}", "ff00ff"); // Magenta
		}
	}

	content.replace("{{data_MHZ19B_co2}}", String(co2data.getRegular()));
	if (co2data.getRegular() > 0)
	{
		if (co2data.getRegular() < 600) // outdoor air
		{
			content.replace("{{descr_MHZ19B_co2}}", "outdoor air");
			content.replace("{{color_MHZ19B_co2}}", "0000ff"); // Blue
		}
		else if (co2data.getRegular() < 800) // fresh indoor air
		{
			content.replace("{{descr_MHZ19B_co2}}", "fresh indoor air");
			content.replace("{{color_MHZ19B_co2}}", "00ff00"); // Green
		}
		else if (co2data.getRegular() < 1000) // Indoor air
		{
			content.replace("{{descr_MHZ19B_co2}}", "Indoor air");
			content.replace("{{color_MHZ19B_co2}}", "adff2f"); // GreenYellow
		}
		else if (co2data.getRegular() < 1200) // used indoor air. please ventilate
		{
			content.replace("{{descr_MHZ19B_co2}}", "used indoor air. please ventilate");
			content.replace("{{color_MHZ19B_co2}}", "ffff00"); // Yellow
		}
		else if (co2data.getRegular() < 1400) // stale indoor air. please ventilate
		{
			content.replace("{{descr_MHZ19B_co2}}", "stale indoor air. please ventilate");
			content.replace("{{color_MHZ19B_co2}}", "ffa500"); // Orange
		}
		else if (co2data.getRegular() < 1600) // strongly stale indoor air. please ventilate urgently. thinking performance impaired
		{
			content.replace("{{descr_MHZ19B_co2}}", "strongly stale indoor air. please ventilate urgently. thinking performance impaired");
			content.replace("{{color_MHZ19B_co2}}", "ff0000"); // Red
		}
		else // Tiredness, headache. please ventilate urgently.
		{
			content.replace("{{descr_MHZ19B_co2}}", "Warning. Tiredness, headache. please ventilate urgently.");
			content.replace("{{color_MHZ19B_co2}}", "ff00ff"); // Magenta
		}
	}

	layout.replace("{{content}}", content);
	request->send(200, "text/html; charset=utf-8", layout);
}

void WebServerHandler::handle_page_setting(AsyncWebServerRequest *request)
{
	String content = html_settings;
	String css = html_css;
	String name = "Turtle Settings";
	String layout = html_layout;

	layout.replace("{{unifiedCSS}}", css);
	layout.replace("{{name}}", name);
	layout.replace("{{title}}", name);

	// Load current settings from preferences
	preferences.begin("config", true);
	bool switchWiFiCheck = preferences.getBool("switchWIFI", switch_WIFI);
	bool switchRAMPrintout = preferences.getBool("switchPRINT", switch_PRINT);
	bool switchEPD = preferences.getBool("switchEPD", switch_EPD);
	bool switchLED = preferences.getBool("switchLED", switch_LED);
	bool switchmqtt = preferences.getBool("switchMQTT", switch_MQTT);
	bool switchdebug = preferences.getBool("switchDEBUG", DEBUG);

	int intervalMHZ19inSeconds = preferences.getInt("intervalMHZ19", interval_MHZ19_in_Seconds);
	int intervalBME680inSeconds = preferences.getInt("intervalBME680", interval_BME680_in_Seconds);
	int intervalWiFiCheckinSeconds = preferences.getInt("intervalWiFi", interval_WiFiCheck_in_Seconds);
	int intervalRAMPrintoutinSeconds = preferences.getInt("intervalPRINT", interval_RAMPrintout_in_Seconds);
	int intervalEPDinSeconds = preferences.getInt("intervalEPD", interval_EPD_in_Seconds);
	int intervalLEDinSeconds = preferences.getInt("intervalLED", interval_LED_in_Seconds);
	int intervalmqttinSeconds = preferences.getInt("intervalMQTT", interval_mqtt_in_Seconds);
	preferences.end();

	Serial.println("Current read from config Switch Settings:");
	Serial.printf("WiFi: %s\n", switchWiFiCheck ? "ON" : "OFF");
	Serial.printf("Print: %s\n", switchRAMPrintout ? "ON" : "OFF");
	Serial.printf("EPD: %s\n", switchEPD ? "ON" : "OFF");
	Serial.printf("LED: %s\n", switchLED ? "ON" : "OFF");
	Serial.printf("MQTT: %s\n", switchmqtt ? "ON" : "OFF");
	Serial.printf("DEBUG: %s\n", switchdebug ? "ON" : "OFF");

	content.replace("{{switchWIFI_checked}}", switchWiFiCheck ? "checked" : "");
	content.replace("{{switchPRINT_checked}}", switchRAMPrintout ? "checked" : "");
	content.replace("{{switchEPD_checked}}", switchEPD ? "checked" : "");
	content.replace("{{switchLED_checked}}", switchLED ? "checked" : "");
	content.replace("{{switchMQTT_checked}}", switchmqtt ? "checked" : "");
	content.replace("{{switchDEBUG_checked}}", switchdebug ? "checked" : "");

	content.replace("{{interval_MHZ19_in_Seconds}}", String(intervalMHZ19inSeconds));
	content.replace("{{interval_BME680_in_Seconds}}", String(intervalBME680inSeconds));
	content.replace("{{interval_WiFiCheck_in_Seconds}}", String(intervalWiFiCheckinSeconds));
	content.replace("{{interval_RAMPrintout_in_Seconds}}", String(intervalRAMPrintoutinSeconds));
	content.replace("{{interval_EPD_in_Seconds}}", String(intervalEPDinSeconds));
	content.replace("{{interval_LED_in_Seconds}}", String(intervalLEDinSeconds));
	content.replace("{{interval_mqtt_in_Seconds}}", String(intervalmqttinSeconds));

	layout.replace("{{content}}", content);
	request->send(200, "text/html", layout);
}

void WebServerHandler::handle_submit_credentials(AsyncWebServerRequest *request)
{
	preferences.begin("config", false); 
	preferences.putString("wlanSSID", request->getParam("wlanSSID", true)->value());
	preferences.putString("wlanPASSWORD", request->getParam("wlanPASSWORD", true)->value());
	preferences.end();
	ESP.restart();
}

void WebServerHandler::handle_submit_setting(AsyncWebServerRequest *request)
{
	preferences.begin("config", false);

	if (request->hasParam("intervalMHZ19"))
	{
		preferences.putInt("intervalMHZ19", request->getParam("intervalMHZ19")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalMHZ19", interval_MHZ19_in_Seconds);
	}

	if (request->hasParam("intervalBME680"))
	{
		preferences.putInt("intervalBME680", request->getParam("intervalBME680")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalBME680", interval_BME680_in_Seconds);
	}

	if (request->hasParam("intervalWiFi"))
	{
		preferences.putInt("intervalWiFi", request->getParam("intervalWiFi")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalWiFi", interval_WiFiCheck_in_Seconds);
	}

	if (request->hasParam("intervalPRINT"))
	{
		preferences.putInt("intervalPRINT", request->getParam("intervalPRINT")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalPRINT", interval_RAMPrintout_in_Seconds);
	}

	if (request->hasParam("intervalEPD"))
	{
		preferences.putInt("intervalEPD", request->getParam("intervalEPD")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalEPD", interval_EPD_in_Seconds);
	}

	if (request->hasParam("intervalLED"))
	{
		preferences.putInt("intervalLED", request->getParam("intervalLED")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalLED", interval_LED_in_Seconds);
	}

	if (request->hasParam("intervalMQTT"))
	{
		preferences.putInt("intervalMQTT", request->getParam("intervalMQTT")->value().toInt());
	}
	else
	{
		preferences.putInt("intervalMQTT", interval_mqtt_in_Seconds);
	}

	preferences.end();
	request->send(200, "text/plain", "Interval Settings Saved!");
	request->redirect("/");
}

void WebServerHandler::handle_submit_switch(AsyncWebServerRequest *request)
{
	preferences.begin("config", false);


		request->getParam("switchWIFI")->value();

		bool switchWIFIValue = request->hasParam("switchWIFI") && request->getParam("switchWIFI")->value() == "on";
		preferences.putBool("switchWIFI", switchWIFIValue);

		bool switchPRINTValue = request->hasParam("switchPRINT") && request->getParam("switchPRINT")->value() == "on";
		preferences.putBool("switchPRINT", switchPRINTValue);

		bool switchEPDValue = request->hasParam("switchEPD") && request->getParam("switchEPD")->value() == "on";
		preferences.putBool("switchEPD", switchEPDValue);

		bool switchLEDValue = request->hasParam("switchLED") && request->getParam("switchLED")->value() == "on";
		preferences.putBool("switchLED", switchLEDValue);

		bool switchMQTTValue = request->hasParam("switchMQTT") && request->getParam("switchMQTT")->value() == "on";
		preferences.putBool("switchMQTT", switchMQTTValue);

		bool switchDEBUGValue = request->hasParam("switchDEBUG") && request->getParam("switchDEBUG")->value() == "on";
		preferences.putBool("switchDEBUG", switchDEBUGValue);
	preferences.end();
	request->redirect("/");
}

void WebServerHandler::handle_reset(AsyncWebServerRequest *request)
{
	request->send(200, "text/html", "Device is resetting...");
	delay(500);
	ESP.restart();
}

void WebServerHandler::handle_load_defaults(AsyncWebServerRequest *request)
{
	settingsHandler.reset(); // Load default settings
	request->redirect("/");
	request->send(200, "text/plain", "Defaults loaded");
	delay(500);
	ESP.restart();
}

void WebServerHandler::handle_page_NotFound(AsyncWebServerRequest *request)
{
	request->send(404, "text/plain; charset=utf-8", "Not found");
}

void WebServerHandler::setInputDataforBody(DataCO2 co2Sensordata, Bsec enviromentdata, String sdate)
{
	this->bmedata = enviromentdata;
	this->co2data = co2Sensordata;
	acDate = sdate;
}

void WebServerHandler::start()
{
	server.begin();
}
