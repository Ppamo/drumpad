const int A2D[] = {13, 12, 11, 10, 9, 8, 2, 3};
const int THRESHOLD = 120;
const int PLAYING_DELAY = 60;
byte val = 0;
int playing[] = {0, 0, 0, 0, 0, 0, 0, 0};
int lastHit = 500;
int counter = 0;
int pushButtonCounter = 0;
int *loopArrayPad = 0;
int *loopArrayCount = 0;
int loopArraySize = 0;
int pushButtonPin = 4;
int currentMenu = 0;


void setup() {
  for (int i = 0; i < 8; i++){
    pinMode(A2D[i], OUTPUT);
  }
  pinMode(pushButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // Check the pads
  readPads();

  // play rhythm
  playRhythm();
  
  // check the button events
  readEventButton();

  if (currentMenu == 2)
    recordLoop(); // read pads and store it in the loop structures
  if (currentMenu == 3)
    playLoop(); // play the record loop

  counter++;
  delay(1);
}

void readPads(){
  for (int i = 0; i < 6; i++){
    val = analogRead(i);
    if (val >= THRESHOLD && playing[i] == 0){
      playing[i] = PLAYING_DELAY;
      digitalWrite(A2D[i], HIGH);
      if (currentMenu == 1) { // recordRhythm
        lastHit = counter;
        counter = 0;
      }
      // Serial.write(i);
      // Serial.write(val);
    }
    if (playing[i] > 0){
      playing[i]--;
      if (playing[i] == 0)
        digitalWrite(A2D[i], LOW);
    }
  }
}

void playRhythm() {
  if ((counter % lastHit) == 0){
    digitalWrite(A2D[6], HIGH);
    playing[6] = PLAYING_DELAY;
  }
  if ((counter % (lastHit * 4)) == 0){
    digitalWrite(A2D[7], HIGH);
    playing[7] = PLAYING_DELAY;
    counter = 0;
  }
  // clean the leds for rhythm if there are in HIGH mode
  for (int i = 6; i < 8; i++){
    if (playing[i] > 0){
      playing[i]--;
      if (playing[i] == 0)
        digitalWrite(A2D[i], LOW);
    }
  }
}

void recordLoop() {
  Serial.println("recording loop!");
}

void playLoop() {
  Serial.println("playing loop!");
}

void readEventButton(){
  int event = checkButton();
  if (event == 1) // click
    buttonClick();
  if (event == 2) // hold
    buttonHold();
  if (event == 3) // double click
    buttonSuperHold();
}

int checkButton(){
  if (pushButtonCounter > 0){
    pushButtonCounter++;
    if ((pushButtonCounter % 50) == 0) {
      if (digitalRead(pushButtonPin) == LOW)
        return 0;
        
      if (pushButtonCounter < 500) {
        pushButtonCounter = 0;
        return 1;
      }
      if (pushButtonCounter < 1250) {
        pushButtonCounter = 0;
        return 2;
      }
      pushButtonCounter = 0;
      return 3;
    }
  } else {
    if (digitalRead(pushButtonPin) == LOW){
      Serial.println("iniciando eventos");
      pushButtonCounter++;
    }
  }
  return 0;
}

void saveHitInfo(int pad, int count, int value){
  // add a new step into the array
  if (loopArrayPad != 0) {
    loopArrayPad = (int*) realloc(loopArrayPad, loopArraySize * sizeof(int));
    loopArrayCount = (int*) realloc(loopArrayCount, loopArraySize * sizeof(int));
  } else {
    loopArrayPad = (int*) malloc(loopArraySize * sizeof(int));
    loopArrayCount = (int*) malloc(loopArraySize * sizeof(int));
  }
  loopArrayPad[loopArraySize] = pad;
  loopArrayCount[loopArraySize] = count;
  loopArraySize++;
}

void clearHitArray(){
  free(loopArrayPad);
  free(loopArrayCount);
  loopArrayPad = 0;
  loopArrayCount = 0;
  loopArraySize = 0;
}

void menuChange() {
  for (int i = 0; i < 6; i++)
    digitalWrite(A2D[i], HIGH);
  delay (20);
  for (int i = 0; i < 6; i++)
    digitalWrite(A2D[i], LOW);
}

void buttonClick(){
  Serial.println("button click!");
}

void buttonHold(){
  Serial.println("button hold!");
}

void buttonSuperHold(){
  Serial.println("button super hold!");
  menuChange();
  currentMenu++;
  if (currentMenu == 2) currentMenu = 0;
}
