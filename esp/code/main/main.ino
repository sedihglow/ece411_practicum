#include "hw.h"
#include "moisture.h"
#include "external_led.h"
#include "water_sensor.h"
#include "pump.h"
#include "esp_eeprom.h"
#include "irrigator_wifi.h"

Adafruit_seesaw moisture_sensor; // moisture sensor class
data_s edata; // eeprom data for operation

uint16_t moisture_read = 0;
int water_read = 0;
int led_blink = HIGH;

WiFiUDP udp;
unsigned int local_udp_port = UDP_PORT; // listens on this port
char incoming_packet[PACKET_BUFF_SIZE] = {'\0'}; // buffer for incoming packets
int packet_size = 0;
int len = 0;

void setup()
{
    int ret = SUCCESS;
    int start_addr = 0;
    data_s init_test_data; // used to test if eeprom is initialized

    Serial.begin(BAUD_RATE);
    Serial.print('\n'); // newline for garbage terminal produces on boot
    
    // initialize data with default values
    init_eeprom_data(&edata);

    // if eeprom is uninitialized, initialize it.
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(start_addr, init_test_data);
    if (init_test_data.pump_time > UNINIT_PUMP_VALUE_THRESH ||
        init_test_data.pump_time < 1) {
        Serial.println("storing init edata to eeprom");
        EEPROM.put(start_addr, edata);
        EEPROM.commit();
    }

    // setup external led pin to output
    setup_led_out();

    // setup i2c moisture sensor
    ret = setup_moisture_sensor(&moisture_sensor);
    if (ret) {
        Serial.println("ERROR  moisture sensor not found");
        while(true); // if no moisture sensor do nothing
    } else {
        Serial.println("Success, moisture sensor found");
    }

    // setup water sensor as input
    setup_water_sensor();
    Serial.println("water sensor pin setup");

    // setup pump pin as output
    setup_pump_pin();
    Serial.println("pump pin setup");

    // setup wifi, if wifi not connected creates hotspot which can change
    // ssid and password, saves new credentials to eeprom.
    setup_wifi();

    // print local ip address of ESP
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    udp.begin(local_udp_port);
    Serial.printf("Now listening at IP %s, UDP port %d\n", 
                  WiFi.localIP().toString().c_str(), local_udp_port);
}

void loop()
{
    set_led(&led_blink);

    // check if we have any messages from user
    packet_size = udp.parsePacket();
    if (packet_size) {
        Serial.printf("Received %d bytes from %s, port %d\n",
                      packet_size, udp.remoteIP().toString().c_str(), 
                      udp.remotePort());
        len = udp.read(incoming_packet, PACKET_BUFF_SIZE);
        if (len > 0)
            incoming_packet[len] = '\0';
        
        handle_user_request(incoming_packet);
    }


    moisture_read = read_moisture(&moisture_sensor); 
    Serial.println("moisture reading: " +String(moisture_read));

    water_read = read_water_sensor();
    if (!water_read)
        Serial.println("water level low");
    
    // if there is still water in the tank and moisture is low
    if (water_read && moisture_read < edata.moisture_thresh) {
        Serial.println("water being pumped for " +String(edata.pump_time)
                     +" seconds");

        // start water pump for x seconds
        pump_water(edata.pump_time);
        delay(5000); // wait for soil to absorb water
    }

    delay(1000);
}
