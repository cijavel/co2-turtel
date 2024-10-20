const char *html_index = R"rawliteral(
    <html>
        <head>
            <title>{{name}}</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
                {{unifiedCSS}}
            </style>
        </head>
        <body>
            <div class="container">
                <h1>{{title}}</h1>
                <div class="nav">
                    <button class="button" onclick="location.href='/status'">Status</button>
                    <button class="button" onclick="location.href='/json'">JSON File</button>
                    <button class="button" onclick="location.href='/WLAN'">WLAN Settings</button>
                    <button class="button" onclick="location.href='/settings'">Settings</button>
                </div>
            </div>
            <div class="footer">
                &copy; 2024 SensorTurtle
            </div>
        </body>
    </html>
)rawliteral";