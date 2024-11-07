#include <Arduino.h>
int * createSequence(int sequence[]) {

  return sequence;
}
int displaySequence(int sequence) {
  
}
void flashLights() {

}
bool playerTurn(int sequence) {

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
  int sequence = {};
  bool roundWon = true;
  while (roundWon == true)
    int nextInt = random(4);
    int sequenceLength = sizeof(sequence);
    sequence[sequenceLength] = nextInt;
    displaySequence(sequence);
    flashLights();
  bool roundWon = playerTurn(sequence);
  if (roundWon == true) {
    flashLights();
  }
}

