#define WLS_PIN 4

int wlsRead = LOW;

void setup()
{
    Serial.begin(115200);
    Serial.println("Water level sensor"); 
    pinMode(WLS_PIN, INPUT);
}

void loop()
{
    wlsRead = digitalRead(WLS_PIN);
    Serial.println(wlsRead);
    delay(2000);
}
