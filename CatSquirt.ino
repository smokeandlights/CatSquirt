/* 
This is the sketch for a cat deterrent system to try to get the outdoor cats in the neighborhood to avoid being under my deck.
I am connecting the output of this to a 12v solenoid valve tied into my garden hose and some kind of sprayer attachment. 
*/
const int pirpin = 3;   //PIR Motion Detector
const int valvepin = 4; //TIP122 (because I have those on hand) transistor to trigger a 12v solenoid valve
const int greenled = 5; //System ARMED
const int redled = 13;  //Waiting to rearm trigger - 
const int temppin = A0; //TMP36 temperature sensor
const int repeatDelay = 10000; //avoid constant triggering - the PIR triggers for about 2 seconds at the lowest setting.
int squirt = 250;       //length of time the valve will be opened each time it is triggered
unsigned long lastShot = 0; //timekeeping on the most recent shot
int shotsFired = 0;     //keeping track of shots fired
int inByte = 0;         //for serial, to get a response when idle

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

  if (millis() >= lastShot + (repeatDelay)) { 

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
        Serial.print(lastShot / 1000);
        Serial.print(" seconds of runtime." );
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
