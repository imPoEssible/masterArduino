int state = 0; // Active = 1, Passive = 0
int counter = 0; // counts knocks up to 3 
int impactval;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
delay(30);
impactval = analogRead(A0);
Serial.print(impactval);
Serial.print(", ");
Serial.print(state);
Serial.print(", ");
Serial.println(counter);

if (impactval > 500){
    delay(100);
    counter = counter + 1;
  if (counter == 3){
    if (state == 0){
    counter = 0;
    state = 1;
    }
    else if (state == 1){
    counter = 0;
    state = 0;
    }
    }
  }
}
