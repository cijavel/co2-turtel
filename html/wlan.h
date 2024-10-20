const char *html_wlan = R"rawliteral(
<h3>Saved Credentials:</h3>
<table class="wlan"><tbody>
    <tr>
        <th>SSID:</th>
        <td>{{ssid}}</td>
    </tr><tr>
        <th>Password:</th>
        <td>
            <span id="saved-password">********</span><br>
            <button type="button" onmousedown="showSavedPassword()" onmouseup="hideSavedPassword()" onmouseleave="hideSavedPassword()">Show</button>
        </td>
    </tr>
</tbody></table>
<h3>Change Credentials:</h3>
<form action="/submit" method="POST">
    <table><tbody>
        <tr>
            <th><label for="wlanSSID">SSID: </label></th>
            <td><input type="text" name="wlanSSID"></td>
        </tr><tr>
            <th><label for="wlanPASSWORD">Password:</label></th>
            <td>
                <input type="wlanPASSWORD" name="wlanPASSWORD" id="wlanPASSWORD">
                <button type="button" onmousedown="showPassword()" onmouseup="hidePassword()" onmouseleave="hidePassword()">Show</button>
            </td>
        </tr>
    </tbody></table><br>
    <input class="button" type="submit" value="Submit">
</form>
<script>
    function showPassword() {document.getElementById('password').type = 'text';}
    function hidePassword() {document.getElementById('password').type = 'password';}
    function showSavedPassword() {document.getElementById('saved-password').innerText = '{{password}}';}
    function hideSavedPassword() {document.getElementById('saved-password').innerText = '********';}
</script>
)rawliteral";