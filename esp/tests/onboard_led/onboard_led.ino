#define OB_LED_PIN 2

void setup()
{
    pinMode(OB_LED_PIN, OUTPUT);
}

void loop()
{
    digitalWrite(OB_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(OB_LED_PIN, LOW);
    delay(1000);
}
