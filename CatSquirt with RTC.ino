#include <RTClib.h>

const int pirpin = 3;
const int valvepin = 4;
const int greenled = 5;
const int redled = 13;
const int temppin = A0;
const int repeatDelay = 10000;
int squirt = 250;
unsigned long lastShot = 0;
int shotsFired = 0;
int inByte = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup() {
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(valvepin, OUTPUT);
  pinMode(pirpin, INPUT);
  Serial.begin(9600);
  Serial.println("Cat Squirt 2000");
  digitalWrite(redled, HIGH);
  Serial.println("ARMING");
  delay(repeatDelay); // just some time to get out of the way.
  Serial.println("ARMED" );
}

void loop() {
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte /= Serial.read();

    Serial.print("Shots fired since last reset: ");
    Serial.print(shotsFired);
    Serial.print(". Last shot fired at " );
    Serial.print(lastShot / 1000);
    Serial.print(" seconds of runtime." );
    int tempreading = analogRead(temppin);
    float voltage = tempreading * 5.0;
    voltage /= 1024.0;
    float tempC = (voltage - 0.5) * 100;
    float tempF = (tempC * 9.0 / 5.0) + 32.0;
    Serial.print(" Current Ambient Temperature: " );
    Serial.print(tempF);
    Serial.println(" degrees F" );
  }

  if (millis() >= lastShot + (repeatDelay)) { //avoid constant triggering - the PIR triggers for about 2 seconds at the lowest setting.

    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);


    if ( digitalRead(pirpin) == HIGH) { //detect motion
      int tempreading = analogRead(temppin);
      float voltage = tempreading * 5.0;
      voltage /= 1024.0;
      float tempC = (voltage - 0.5) * 100;
      float tempF = (tempC * 9.0 / 5.0) + 32.0;

      if (tempF >= 55.0) {
        digitalWrite(valvepin, HIGH);
        digitalWrite(redled, HIGH);
        digitalWrite(greenled, LOW);
        delay(squirt);
        digitalWrite(valvepin, LOW);
        shotsFired++;
        lastShot = millis();
        Serial.print("Shot " );
        Serial.print(shotsFired);
        Serial.print(" fired at " );
       DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
//    Serial.print(" (");
//    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
//    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
        Serial.print(" Ambient Temperature: " );
        Serial.print(tempF);
        Serial.println(" degrees F" );
        //        Serial.println("RELOADING" ); //put this back in if you can figure out how to make "armed" only print once in the loop.
        delay(100);
      }
      else {
        Serial.print("Too Cold! Temperature: " );
        Serial.print(tempF);
        Serial.println("F" );
      }
    }
  }
}
