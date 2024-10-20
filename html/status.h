const char *html_status = R"rawliteral(
    <table class="content">
        <tr>
            <th class="title">Sensor Accuracy</th>
            <td class="data">{{data_iaqaccuracy}} level</td>
            <td class="status" style="background-color: #{{color_iaqaccuracy}};"> </td>
            <td class="description">{{descr_iaqaccuracy}}</td>
        </tr>
        <tr>
            <th class="title">Temperature</th>
            <td class="data">{{data_temp}} °C</td>
            <td class="status" style="background-color: #{{color_temp}}; "> </td>
            <td class="data">{{descr_temp}}</td>
        </tr>
        <tr>
            <th class="title">Humidity</th>
            <td class="data">{{data_relativehumidity}} %</td>
            <td class="status" style="background-color: #{{color_relativehumidity}};"> </td>
            <td class="data">{{descr_relativehumidity}}</td>
        </tr>
        <tr>
            <th class="title">Air Quality </th>
            <td class="data">{{data_iaq}} IAQ</td>
            <td class="status" style="background-color: #{{color_iaq}};"> </td>
            <td class="data">{{descr_iaq}}</td>
        </tr>
        <tr>
            <th class="title">Co2 Level</th>
            <td class="data">{{data_MHZ19B_co2}} ppm</td>
            <td class="status" style="background-color: #{{color_MHZ19B_co2}};"> </td>
            <td class="data">{{descr_MHZ19B_co2}}</td>
        </tr>
        <tr>
            <th class="title"> ..... </th>
        </tr>
        <tr>
            <th class="title">Air pressure</th>
            <td class="data">{{data_pressure}} hPa</td>
        </tr>
        <tr>
            <th class="title">Gas resistance</th>
            <td class="data">{{data_gas}} Ohm</td>
        </tr>
        <tr>
            <th class="title">breath VOC Equivalent</th>
            <td class="data">{{data_breahtvoc}} ppm</td>
        </tr>
        <tr>
            <th class="title"> ..... </th>
        </tr>
        <tr>
            <th class="title">Timestep</th>
            <td class="data">{{data_timestep}}</td>
        </tr>
        <tr>
            <th class="title">Time</th>
            <td class="data">{{data_time}}</td>
        </tr>
        <tr>
            <th class="title">Timezone</th>
            <td class="data">{{data_zone}}</td>
        </tr>
    </table>
)rawliteral";