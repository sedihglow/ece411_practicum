#define E_LED_PIN 12

void setup()
{
    pinMode(OB_LED, OUTPUT);
}

void loop()
{
    digitalWrite(OB_LED, HIGH);
    delay(1000);
    digitalWrite(OB_LED, LOW);
    delay(1000);
}
