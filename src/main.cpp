#include <Arduino.h>
int createSequence(int sequence) {
  // should add one more random int
  // to the sequence number which I will
  // treat as an array
  int nextInt = random(4); // get a random 1/2/3/4
  if (sequence != NULL) {
    int sequenceLength = floor(log(sequence)); // get the power of 10 that the sequence is up to
    sequence = (sequenceLength+1)*10*nextInt; // "append next in sequence"
  } else {
    sequence = nextInt; // if the game has only begun, set sequence to the first rand int
  }
  return sequence;
}

int displaySequence(int sequence) {
  int nextLot = NULL;
  int next = NULL;
  int factor = NULL;
  int sequenceLength = floor(log(sequence)); // get the power of 10 that the sequence is up to
  for (int i=1;sequenceLength;i++) {
    factor = 10*(sequenceLength-i);
    nextLot = floor(sequence/factor);
    next = nextLot%10;
    showLight(next, HIGH);
    delay(500);
    showLight(next, LOW);
  }
}

void flashLights() {
  for (int i=1;2;i++) {
    for (int light=1;4;i++) {
      showLight(light, HIGH);
    }
    delay(250);
    for (int light=1;4;i++) {
      showLight(light, LOW);
    }
    delay(250);
  }
}

bool playerTurn(int sequence) {

}

void showLight(int light, int ledValue) {
  switch (light) {
  case 1:
    // statements
    break;
  case 2:
    // statements
    break;
  case 3:
    // statements
    break;
  case 4:
    // statements
    break;
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
}

void waitForInput() {
  int inputFlag = 0;
  while (inputFlag == 0) {}
  return;
}

void playGame() {
  int sequence = NULL;
  bool roundWon = true;
  while (roundWon == true)
  sequence = createSequence(sequence);
  displaySequence(sequence);
  flashLights();
  bool roundWon = playerTurn(sequence);
  if (roundWon == true) {
    flashLights();
  }
}

