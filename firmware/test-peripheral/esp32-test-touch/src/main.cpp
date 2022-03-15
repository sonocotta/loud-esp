#include <Arduino.h>

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;

  delay(1000);

  Serial.println("\n\nCapacitive touch sensors test..");
}

void loop(void)
{
      Serial.printf("%d / %d / %d / %d / %d / %d / %d / %d / %d / %d \n", 
      touchRead(T0), 
      touchRead(T1),
      touchRead(T2), 
      touchRead(T3),
      touchRead(T4), 
      touchRead(T5),
      touchRead(T6), 
      touchRead(T7),
      touchRead(T8), 
      touchRead(T9)
    );
  delay(100);
}