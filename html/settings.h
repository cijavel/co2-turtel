const char *html_settings = R"rawliteral(
    <form action="/submitswitch" method="POST">
        <table class="content">
            <tr>
                <th><label for="switchWIFI">WiFi Check</label></th>
                <td>
                    <input type="checkbox" id="switchWIFI" name="switchWIFI" value="on" {{switchWIFI_checked}}  title="Check WLAN status">
                </td>
            </tr>
            <tr>
                <th><label for="switchPRINT">RAM Printout</label></th>
                <td>
                    <input type="checkbox" id="switchPRINT" name="switchPRINT" value="on" {{switchPRINT_checked}} title="Display RAM on console">
                </td>
            </tr>
            <tr>
                <th><label for="switchEPD">EPD Display</label></th>
                <td>
                    <input type="checkbox" id="switchEPD" name="switchEPD" value="on" {{switchEPD_checked}} title="Display sensor data on EPD">
                </td>
            </tr>
            <tr>
                <th><label for="switchLED">LED</label></th>
                <td>
                    <input type="checkbox" id="switchLED" name="switchLED" value="on" {{switchLED_checked}} title="Display sensor data on LED strips">
                </td>
            </tr>
            <tr>
                <th><label for="switchMQTT">MQTT</label></th>
                <td>
                    <input type="checkbox" id="switchMQTT" name="switchMQTT" value="on" {{switchMQTT_checked}} title="Send sensor data via MQTT">
                </td>
            </tr>
            <tr>
                <th><label for="switchDEBUG">Debug message</label></th>
                <td>
                    <input type="checkbox" id="switchDEBUG" name="switchDEBUG" value="on" {{switchDEBUG_checked}} title="Display debug messages in console">
                </td>
            </tr>
        </table>
        <input type="submit" value="Save Settings">
    </form>

    <form action="/submitsetting" method="POST">
            <table class="content">
            <tr>
                <th><label for="intervalMHZ19"19>MHZ19 Interval (s)</label></th>
                <td><input type="number" id="intervalMHZ19" name="intervalMHZ19" value="{{interval_MHZ19_in_Seconds}}"></td>
            </tr><tr>
                <th><label for="intervalBME680">BME680 Interval (s)</label></th>
                <td><input type="number" id="intervalBME680" name="intervalBME680" value="{{interval_BME680_in_Seconds}}"></td>
            </tr><tr>
                <th><label for="intervalWiFi">WiFi Check Interval (s)</label></th>
                <td><input type="number" id="intervalWiFi" name="intervalWiFi" value="{{interval_WiFiCheck_in_Seconds}}"></td>
            </tr><tr>
                <th><label for="intervalPRINT">RAM Printout Interval (s)</label></th>
                <td><input type="number" id="intervalPRINT" name="intervalPRINT" value="{{interval_RAMPrintout_in_Seconds}}"></td>
            </tr><tr>
                <th><label for="intervalEPD">EPD Refresh Interval (s)</label></th>
                <td><input type="number" id="intervalEPD" name="intervalEPD" value="{{interval_EPD_in_Seconds}}"></td>
            </tr><tr>
                <th><label for="intervalLED">LED Update Interval (s)</label></th>
                <td><input type="number" id="intervalLED" name="intervalLED" value="{{interval_LED_in_Seconds}}"></td>
            </tr><tr>
                <th><label for="intervalMQTT">MQTT Interval (s)</label></th>
                <td><input type="number" id="intervalMQTT" name="intervalMQTT" value="{{interval_mqtt_in_Seconds}}"></td>
            </tr>
        </table>
        <!-- Submit Button -->
        <input type="submit" value="Save Settings">
    </form>

    <!-- Reset Button -->
    <form action="/reset" method="POST">
        <button type="submit">Reset ESP32</button>
    </form>
    <!-- Reset Button -->
    <form action="/load_defaults" method="POST">
        <button onclick="loadDefaults()">Load Default Settings</button>
    </form>

)rawliteral";