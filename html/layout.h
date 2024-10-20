const char *html_layout = R"rawliteral(
    <html>
        <head>
            <title>{{name}}</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>{{unifiedCSS}}</style>
        </head>
        <body>
            <div class="container">
                <div class="nav">
                    <button class="button" onclick="window.location.href='/'">Back to Index</button>
                </div>
                <h1>{{title}}</h1>
                {{content}}
            </div>
            <div class="footer">&copy; 2024 SensorTurtle</div>
        </body>
    </html>
)rawliteral";