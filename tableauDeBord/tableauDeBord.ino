#include <DHT.h>
#include <LiquidCrystal.h>

#define BUTTON 2
#define BUZZER 3
#define PIN_FAN 8
#define PIN_ECHO 9
#define PIN_TRIGER 10

#define LED_BLUE A0
#define LED_RED A1
#define LED_GREEN A2
#define LED_YELLOW A3
#define BRIGHTNESS A4

DHT DHT(13, DHT11);
LiquidCrystal LCD(12, 11, 4, 5, 6, 7);

const int LCD_NB_ROWS = 2;
const int LCD_NB_COLUMNS = 16;

const int PWM_LOW = 0;
const int PWM_HIGH = 255;

const int FREQUENCY = 255;

int buttonCount = 0;
int buttonState = 0;

int brightnessValue = 0;

int distanceCm;
long duration;


void startMenu() {
  LCD.setCursor(0, 0);
  LCD.print("Press the button");
  LCD.setCursor(0, 1);
  LCD.print("To change mode");
}

void getTemperatureAndHumidity() {
  float humidity = DHT.readHumidity();
  float temperature = DHT.readTemperature();

  if (10 <= temperature && temperature <= 18) {
    analogWrite(LED_BLUE, PWM_HIGH);
  } else if (19 <= temperature && temperature <= 21) {
    analogWrite(LED_RED, PWM_HIGH);
  } else if (222 <= temperature) {
    digitalWrite(PIN_FAN, HIGH);
  } else {
    analogWrite(LED_RED, PWM_LOW);
    analogWrite(LED_BLUE, PWM_LOW);
    digitalWrite(PIN_FAN, LOW);
  }

  if (humidity <= 40) {
    analogWrite(LED_GREEN, PWM_HIGH);
  } else {
    analogWrite(LED_YELLOW, PWM_HIGH);
  }

  LCD.setCursor(0, 0);
  LCD.print("Humidity: ");
  LCD.setCursor(0, 1);
  LCD.print("Temp: ");

  LCD.setCursor(LCD_NB_COLUMNS - 5, 0);
  LCD.print(humidity);
  LCD.setCursor(LCD_NB_COLUMNS - 5, 1);
  LCD.print(temperature);
}

void getDistanceAndBuzz() {
  digitalWrite(PIN_TRIGER, LOW);
  delay(5);
  digitalWrite(PIN_TRIGER, HIGH);
  delay(10);
  digitalWrite(PIN_TRIGER, LOW);

  duration = pulseIn(PIN_ECHO, HIGH);
  distanceCm = duration * 0.034 / 2;

  if (999 < distanceCm) {
    LCD.setCursor(0, 0);
    LCD.print("Distance: ");
    LCD.print(999);
    LCD.print(F(" cm "));
  } else {
    LCD.setCursor(0, 0);
    LCD.print("Distance: ");
    LCD.print(distanceCm);
    LCD.print(F(" cm  "));
  }

  if (distanceCm < 5) {
    tone(BUZZER, FREQUENCY);
  } else if (distanceCm < 10) {
    tone(BUZZER, FREQUENCY);
    delay(50);
    noTone(BUZZER);
  } else if (distanceCm < 15) {
    tone(BUZZER, FREQUENCY);
    delay(100);
    noTone(BUZZER);
  } else if (distanceCm < 20) {
    tone(BUZZER, FREQUENCY);
    delay(150);
    noTone(BUZZER);
  } else if (distanceCm < 30) {
    tone(BUZZER, FREQUENCY);
    delay(200);
    noTone(BUZZER);
  } else if (distanceCm < 40) {
    tone(BUZZER, FREQUENCY);
    delay(250);
    noTone(BUZZER);
  } else {
    noTone(BUZZER);
  }

  delay(200);
}

void getBrightness() {
  brightnessValue = analogRead(BRIGHTNESS);

  LCD.setCursor(0, 0);
  LCD.print("Brightness: ");
  LCD.print(brightnessValue);
  LCD.print(F(" lux  "));
  LCD.scrollDisplayLeft();
  
  delay(500);
}

void setup() {
  DHT.begin();
  LCD.begin(LCD_NB_COLUMNS, LCD_NB_ROWS);

  pinMode(BUTTON, INPUT);
  pinMode(BRIGHTNESS, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_TRIGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  analogWrite(BUZZER, PWM_LOW);
  analogWrite(LED_RED, PWM_LOW);
  analogWrite(LED_GREEN, PWM_LOW);
  analogWrite(LED_BLUE, PWM_LOW);
  analogWrite(LED_YELLOW, PWM_LOW);

  digitalWrite(PIN_FAN, LOW);
  digitalWrite(PIN_TRIGER, LOW);
}

void loop() {
  buttonState = digitalRead(BUTTON);

  if (buttonState == HIGH) {
    buttonCount++;
    LCD.clear();
    delay(500);
  } else if (3 < buttonCount) {
    buttonCount = 0;
    LCD.clear();
  }

  if (buttonCount == 0) {
    startMenu();
  } else if (buttonCount == 1) {
    getTemperatureAndHumidity();
  } else if (buttonCount == 2) {
    getDistanceAndBuzz();
  } else if (buttonCount == 3) {
    getBrightness();
  }
}
