#include <Narcoleptic.h>

//#define DEBUG

#ifdef DEBUG
#define NARCOLEPTIC_DELAY 10 * 1000L
#else
#define NARCOLEPTIC_DELAY 60 * 60 * 1000L
#endif

#define MAX_INT 32767

const int dividerTop = 5;
const int dividerBottom = 4;
const int readingPin = 3;
const int pumpingPin = 2;
const int pumpPin = 7;

const int moisturePin = 0;
const int moistureMin = 630;

const int dividerDelay = 5 * 1000;
const int pumpDelay = 6 * 1000;

int moisture = 0;

void setup()
{
  pinMode(dividerTop, OUTPUT);
  pinMode(dividerBottom, OUTPUT);
  pinMode(readingPin, OUTPUT);
  pinMode(pumpingPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(moisturePin, INPUT);
  digitalWrite(dividerTop, LOW);
  digitalWrite(dividerBottom, LOW);
  digitalWrite(readingPin, LOW);  
  digitalWrite(pumpingPin, LOW);
  digitalWrite(pumpPin, LOW);
}

void loop()
{
  moisture = read_moisture();
  
  if (moisture < moistureMin) {
    pump();
  }
  
  narcoleptic_sleep(NARCOLEPTIC_DELAY);
}

int read_moisture() {
  int result = 0;
  
  digitalWrite(readingPin, HIGH);
  digitalWrite(dividerTop, LOW);
  digitalWrite(dividerBottom, HIGH);
  delay(dividerDelay);
  
  moisture = analogRead(moisturePin);
  
  digitalWrite(dividerBottom, LOW);
  delay(dividerDelay);  
  digitalWrite(readingPin, LOW);  

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(moisture);
  Serial.println(NARCOLEPTIC_DELAY);
  Serial.end();
#endif

  return result;
}

void pump() {
  digitalWrite(pumpingPin, HIGH);
  digitalWrite(pumpPin, HIGH);
  delay(pumpDelay);
  digitalWrite(pumpingPin, LOW);
  digitalWrite(pumpPin, LOW);
}

//As we need to sleep for a long time and Narcoleptic uses int as the delay, 
//this function takes care of delaying longs.
void narcoleptic_sleep(long miliseconds) {
  while (miliseconds > 0) {
    if (miliseconds > MAX_INT) {
      Narcoleptic.delay(MAX_INT);
      miliseconds -= MAX_INT;
    }
    else {
      Narcoleptic.delay(miliseconds);
      miliseconds = 0;
    }
  }
}
