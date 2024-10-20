const char *html_css = R"rawliteral(

body {
    font-family: Arial, Helvetica, sans-serif;
    margin: 0;
    padding: 0;
    background-color: #f0f0f0;
}
div {
    padding: 5px;
}
h1 {
    font-size: 120%;
}
.container {
    max-width: 960px;
    margin: 0 auto;
    padding: 20px;
    background-color: #ffffff;
    box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);
}
.status {
    border-radius: 20%;
    width: 20px;
    height: 20px;
}
.nav {
    margin-bottom: 20px;
}
table{
    width: 100%;
    margin-top: 20px;
    display: inline-block;
}
td, th, tr, td{
    text-align: left;
    padding: 8px;
    vertical-align: top;
}
th{
    width: 20%;
}
td.data{
    width: 30%;
}
td:has(input), 
table.cred tbody tr td {
  width: 65%;
}
th.title {
    font-weight: bold;
    width: 150px;
}

input {
    padding: 10px;
    font-size: 16px;
    border-radius: 4px;
    border: 1px solid #ccc;
}
button, 
input[type=submit] {
    padding: 10px 20px;
    background-color: #4CAF50;
    color: white;
    font-size: 16px;
    text-align: center;
    cursor: pointer;
    border: none;
    border-radius: 4px;
    margin: 5px 0px;
    display: inline-block;
    border: 1px solid #ccc;
}
input[type=submit]:hover,
button:hover {
    background-color: #45a049;
}

.footer {
    margin-top: 40px;
    text-align: center;
    font-size: 12px;
    color: #666;
}
)rawliteral";