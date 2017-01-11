const int pirpin = 3;
const int valvepin = 4;
const int greenled = 5;
const int redled = 13;
const int temppin = A0;
const int repeatDelay = 10000;
int squirt = 250;
unsigned long currentMillis = 0;
int shotsFired = 0;


void setup() {
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(valvepin, OUTPUT);
  pinMode(pirpin, INPUT);
  Serial.begin(9600);
  Serial.println("Cat Squirt 2000");
  delay(15000);
  digitalWrite(greenled, HIGH);
  Serial.println("ARMED" );
}

void loop() {
  digitalWrite(greenled, HIGH);
  if ( digitalRead(pirpin) == HIGH) {
    int tempreading = analogRead(temppin);
    float voltage = tempreading * 5.0;
    voltage /= 1024.0;
    float tempC = (voltage - 0.5) * 100;
    float tempF = (tempC * 9.0 / 5.0) + 32.0;
    //    Serial.print(" Temperature: " );
    //    Serial.print(tempF);
    //    Serial.println("F" );
    if (tempF >= 55.0) {
      digitalWrite(valvepin, HIGH);
      digitalWrite(redled, HIGH);
      delay(squirt);
      digitalWrite(valvepin, LOW);
      digitalWrite(redled, LOW);
      shotsFired++;
      currentMillis = millis;
      digitalWrite(greenled, LOW);
      Serial.print("Shot " );
      Serial.print(shotsFired);
      Serial.print(" fired at " );
      Serial.print(currentMillis);  // always reads 140?
      Serial.print(" millis" );
      Serial.print(" Temperature: " );
      Serial.print(tempF);
      Serial.println("F" );
      delay(repeatDelay);
    }
    else {
      Serial.print("Too Cold! Temperature: " );
      Serial.print(tempF);
      Serial.println("F" );
    }
  }

}
