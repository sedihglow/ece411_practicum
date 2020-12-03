#include "irrigator_wifi.h"

extern data_s edata; // comes from main.ino used in create_web_server

// create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);

String st;
String content;
int statusCode;

// returns 0 if connected, 1 if hotspot is connected
void setup_wifi()
{
    unsigned int start_addr = 0;

    // try to connect with default wifi credentials
    WiFi.begin(edata.ssid, edata.pass); 
    if (test_wifi()) {
        Serial.println("wifi connected with default credentials");
        return;
    }

    Serial.println("Disconnecting current wifi connection");
    WiFi.disconnect();
    
    // get credentials from eeprom
    Serial.println("Getting credentials from EEPROM");
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(start_addr, edata);

    // connect with new credentials else open hotspot
    WiFi.begin(edata.ssid, edata.pass);
    if (test_wifi()) {
        Serial.println("\nSuccessfully connected");
        return;
    } else {
        Serial.println("Turning on hotspot for credential changes");
        launch_web();
        setup_ap();
    }

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(WAIT_DELAY);
        server.handleClient();
    }
    
}

bool test_wifi()
{
    int count = 0;
    Serial.println("waiting for wifi to connect");
    while (count < CONNECTION_WAIT_COUNT) {
        if (WiFi.status() == WL_CONNECTED)
            return true;
        delay(WAIT_DELAY);
        Serial.print("*");
        ++count;
    }
    Serial.println("");
    Serial.println("connection timeout");
    return false;
}

void launch_web()
{
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
        Serial.println("WiFi connected");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    create_web_server();
    // Start the server
    server.begin();
    Serial.println("Server started");
}

void setup_ap()
{
    int n = 0;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    st = "<ol>";
    for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);
        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
    }
    st += "</ol>";
    delay(100);
    WiFi.softAP("esp8266", "");
    Serial.println("Initializing_softap_for_wifi credentials_modification");
    launch_web();
    Serial.println("over");
}

void create_web_server()
{
  {
    server.on("/", []() {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Welcome to Wifi Credentials Update page";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });
    server.on("/setting", []() {
      strncpy(edata.ssid, server.arg("ssid").c_str(),SSID_SIZE);
      strncpy(edata.pass, server.arg("pass").c_str(),PASS_SIZE);
      if (strlen(edata.ssid) > 0 && strlen(edata.pass) > 0) {
        Serial.println("\nwriting eeprom edata:");
        EEPROM.put(0, edata);
        Serial.println("edata: ssid " + String(edata.ssid) + " pass " +
                        String(edata.pass));
        EEPROM.commit();
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
    });
  }
}
