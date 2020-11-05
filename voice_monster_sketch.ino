
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 9; // Echo Pin of Ultrasonic Sensor
unsigned long lastMillis = 0;
int lastInches = 0; 
bool userWasClose = false;

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   TCCR0B = TCCR0B & B11111000 | B00000010;
   pinMode(2, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(4, OUTPUT);

}

void loop() {

   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration); 
   if (inches>60){ // heartbeat when no ones close
      lastMillis = heartbeat(lastMillis);
   }
   //cm = microsecondsToCentimeters(duration);
   userWasClose = eventOnCloseProximity(inches, userWasClose);
   if  (! userWasClose){
      lastInches = eventOnDistanceDelta(inches, lastInches);
   }
   delay(100);
}


bool eventOnCloseProximity(long currentInches, bool userWasClose){
  if (currentInches < 24){
    Serial.println("Person Close, going wild");
    digitalWrite(2, LOW); //forward
    digitalWrite(4, HIGH); //forward
    int powernum = random(150, 256);
    long delaynum = random(5000, 15000);
    analogWrite(6, powernum);
    Serial.println("Bump power: ");
    Serial.print(powernum);
    Serial.println();
    Serial.println("delay for: ");
    Serial.print(delaynum);
    Serial.println();
    delay(delaynum);
    if (!userWasClose){
         Serial.println("close_sound");
    }
    userWasClose = true;
 } else if (userWasClose){
      Serial.println("User no longer close. Braking");
      digitalWrite(4, LOW); //brake
      userWasClose = false;
      Serial.println("away_sound");
  }
  return userWasClose;
}

long eventOnDistanceDelta(long currentInches, long lastInches){
  long distanceGained = currentInches - lastInches;
  
  if (distanceGained > 0){
    Serial.println("User moving away");
  }
  
  else if (distanceGained < -2 && distanceGained >=-4){
    Serial.println("identified person coming closer, small heartbat");
    Serial.println("closer_sound");
    smallChestBump();
    }


  lastInches = currentInches;
  return lastInches;

  
}

long heartbeat(long lastmilis){
 if (millis() - lastMillis >= 2*60*1000UL)  // every 30 seconds
  {
   lastMillis = millis();  //get ready for the next iteration
   smallChestBump(); //change to periodic bump
   Serial.println("periodic_sound");
  }
  return lastMillis;
  
}

void smallChestBump(){
  digitalWrite(2, LOW); //forward
  digitalWrite(4, HIGH); //forward
  int powernum = random(180, 200);
  long delaynum = random(10000, 30000);
  analogWrite(6, powernum);
 
  Serial.println("Bump power: ");
  Serial.print(powernum);
  Serial.println();
  Serial.println("delay for: ");
  Serial.print(delaynum);
  Serial.println();
  delay(delaynum);
  digitalWrite(4, LOW); //brake
  Serial.println("Bump ended");
  
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
