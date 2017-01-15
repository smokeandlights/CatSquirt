#include <RTClib.h>

const int pirpin = 3;
const int valvepin = 4;
const int greenled = 5;
const int redled = 13;
const int temppin = A0;
const int repeatDelay = 10000;
int squirt = 250;
int shotsFired = 0;
int inByte = 0;
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
DateTime startTime ;
DateTime lastShot ;
int lastShotTemp = 0.00;
unsigned long lastShotMillis = millis();
void setup() {

  Serial.begin(9600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  startTime = rtc.now();

  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(valvepin, OUTPUT);
  pinMode(pirpin, INPUT);

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

    int tempreading = analogRead(temppin);
    float voltage = tempreading * 5.0;
    voltage /= 1024.0;
    float tempC = (voltage - 0.5) * 100;
    float tempF = (tempC * 9.0 / 5.0) + 32.0;
    Serial.println();
    Serial.print("Current Ambient Temperature: " );
    Serial.print(tempF);
    Serial.println(" F" );

//    Serial.println();
    Serial.print("Running since ");
    Serial.print(daysOfTheWeek[startTime.dayOfTheWeek()]);
    Serial.print(", ");

    Serial.print(startTime.month(), DEC);
    Serial.print('/');
    Serial.print(startTime.day(), DEC);
    Serial.print('/');
    Serial.println(startTime.year(), DEC);

    Serial.print("at ");
    Serial.print(startTime.hour(), DEC);
    Serial.print(':');
    Serial.print(startTime.minute(), DEC);
    Serial.print(':');
    Serial.print(startTime.second(), DEC);
    Serial.println();

    Serial.println();
    Serial.print("Shots fired since last reset: ");
    Serial.println(shotsFired);

    Serial.print("Last shot fired on " );
    Serial.print(daysOfTheWeek[lastShot.dayOfTheWeek()]);
    Serial.print(", ");

    Serial.print(lastShot.month(), DEC);
    Serial.print("/");
    Serial.print(lastShot.day(), DEC);
    Serial.print("/");
    Serial.println(lastShot.year(), DEC);


    Serial.print("at ");
    Serial.print(lastShot.hour(), DEC);
    Serial.print(':');
    Serial.print(lastShot.minute(), DEC);
    Serial.print(':');
    Serial.print(lastShot.second(), DEC);
    Serial.println();

    Serial.print("Last Shot Ambient Temp:  ");
    Serial.print(lastShotTemp, DEC);
    Serial.println(" F");


    //    Serial.print(lastShot / 1000);
    //    Serial.print(" seconds of runtime." );

  }

  if (millis() >= lastShotMillis + (repeatDelay)) { //avoid constant triggering - the PIR triggers for about 2 seconds at the lowest setting.

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

        Serial.println();
        Serial.print("Shot " );
        Serial.print(shotsFired);
        Serial.print(" fired at " );
        DateTime now = rtc.now();
        lastShot  = now;
        lastShotMillis = millis();
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();

        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(", ");

        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print('/');
        Serial.print(now.year(), DEC);
        Serial.println(' ');

        Serial.print("Ambient Temperature: " );
        Serial.print(tempF);
        Serial.println(" F" );
        lastShotTemp = tempF;

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
