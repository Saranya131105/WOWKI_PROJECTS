#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define TRIG_PIN 5
#define ECHO_PIN 18

#define GREEN_LED 25
#define YELLOW_LED 26
#define RED_LED 27
#define BUZZER 14



const float tankHeight = 100.0; // cm

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {

  // Send pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  float distance = duration * 0.034 / 2;

  float waterLevel = tankHeight - distance;

  if (waterLevel < 0)
    waterLevel = 0;

  float percentage = (waterLevel / tankHeight) * 100;

  // LCD Display
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Water Level");

  lcd.setCursor(0, 1);
  lcd.print((int)percentage);
  lcd.print("%");

  // Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Level: ");
  Serial.print(percentage);
  Serial.println("%");

  // Turn OFF all first
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  if (percentage > 70) {

    digitalWrite(GREEN_LED, HIGH);

    lcd.setCursor(8, 1);
    lcd.print("FULL");
  }

  else if (percentage >= 30) {

    digitalWrite(YELLOW_LED, HIGH);

    lcd.setCursor(8, 1);
    lcd.print("MID");
  }

  else {

    digitalWrite(RED_LED, HIGH);

    lcd.setCursor(8, 1);
    lcd.print("LOW");

    digitalWrite(BUZZER, HIGH);
  }

  delay(1000);
}