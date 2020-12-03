#define E_LED_PIN 12
#define PUMP_PIN 5


void setup()
{
    Serial.begin(115200);
    Serial.println();
    pinMode(E_LED_PIN, OUTPUT);
    Serial.println("external pin as output");

    pinMode(PUMP_PIN, OUTPUT);
    Serial.println("pump pin as output");
}

void loop()
{
    digitalWrite(E_LED_PIN, HIGH);

    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("pins high");
    delay(2000);
    Serial.println("post high delay");

    digitalWrite(PUMP_PIN, LOW);

    digitalWrite(E_LED_PIN, LOW);
    Serial.println("pins low");
    delay(2000);
    Serial.println("post low delay");
}
