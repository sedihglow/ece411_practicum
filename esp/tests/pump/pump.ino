#define PUMP_PIN 5

int count = 0;

void setup()
{
    pinMode(PUMP_PIN, OUTPUT);
}

void loop()
{
   /*if (count == 0) {
        digitalWrite(PUMP_PIN, HIGH);
        delay(2000);
         ++count;
   }
   */

    digitalWrite(PUMP_PIN, HIGH);
    delay(5000);

    digitalWrite(PUMP_PIN, LOW);
    delay(5000);
}
