#define E_LED_PIN 12

void setup()
{
    pinMode(E_LED_PIN, OUTPUT);
}

void loop()
{
    digitalWrite(E_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(E_LED_PIN, LOW);
    delay(1000);
}
