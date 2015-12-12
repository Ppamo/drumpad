byte val = 0;
int playing[] = {0, 0, 0, 0, 0, 0};
int A2D[] = {13, 12, 11, 10, 9, 8};
int THRESHOLD = 100;
int PLAYING_DELAY = 40;

void setup() {
  for (int i = 0; i < 6; i++){
    pinMode(A2D[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 6; i++){
    val = analogRead(i);
    if (val >= THRESHOLD && playing[i] == 0){
      playing[i] = PLAYING_DELAY;
      digitalWrite(A2D[i], HIGH);
    }
    if (playing[i] > 0){
      playing[i]--;
      if (playing[i] == 0)
        digitalWrite(A2D[i], LOW);
    }
  }
  delay(1);
}
