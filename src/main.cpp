#include <Arduino.h>
static int yellowLED = 4;
static int redLED = 0;
static int blueLED = 2;
static int greenLED = 15;
#define inputThreshold 400


bool waitForInput() {
  int xValue = analogRead(1);
  int yValue = analogRead(3);
  while ((abs(xValue) <  inputThreshold) || (abs(yValue) > inputThreshold)) {
    delay(1);
  }
  return true;
}

void playGame() {
  int sequence = NULL;
  bool roundWon = true;
  while (roundWon == true)
    flashLights();
    sequence = createSequence(sequence);
    displaySequence(sequence);
    flashLights();
    bool roundWon = playerTurn(sequence);
}

int mattsCookedArrayIndexReturn(int array, int index) {
    int arrayLength = floor(log(array)/log(10)); // get the length of the array
    int factor = pow(10, ((arrayLength - index + 1))); // find the multiplier that will put the index in the 1's place
    int arrayChunk = floor(array/factor); // use that multiplier to shrink the number
    int indexValue = arrayChunk%10; // get the 1's place
    return indexValue;
}

int mattsCookedArrayAppend(int array, int nextValue) {
  int arrayLength = floor(log(array)/log(10)); // get the length of the array
  array += pow(10,arrayLength+1)*nextValue;
  return array;
}

int createSequence(int sequence) {
  // should add one more random int
  // to the sequence number which I will
  // treat as an array
  int nextInt = random(4); // get a random 1/2/3/4
  if (sequence != NULL) {
    sequence = mattsCookedArrayAppend(sequence, nextInt); // "append next in sequence"
  } else {
    sequence = nextInt; // if the game has only begun, set sequence to the first rand int
  }
  return sequence;
}

int displaySequence(int sequence) {
  int next;
  int sequenceLength = floor(log(sequence)); // get the power of 10 that the sequence is up to
  for (int i=1;sequenceLength;i++) {
    next = mattsCookedArrayIndexReturn(sequence, i);
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
  int nextLot = NULL;
  int next = NULL;
  int factor = NULL;
  int input = NULL;
  bool roundWon = 1;
  int sequenceLength = floor(log(sequence)); // get the power of 10 that the sequence is up to
  for (int i=1;sequenceLength;i++) {
    input = getPlayerInput();
    factor = 10*(sequenceLength-i);
    nextLot = floor(sequence/factor);
    next = nextLot%10;
    if (input != next) {
      roundWon = 0;
      return roundWon;
    }
  }
  flashLights();
}

int getPlayerInput() { // verify that this outputs the correct things, otherwise change the numbers
  int input = 0;
  while (!waitForInput()) {
    delay(1);
  }
  int xValue = analogRead(1);
  int yValue = analogRead(3);
  if (abs(xValue) > abs(yValue)) {
    if (xValue > 0) {
      input = 2;
    } else {
      input = 4;
    }
  } else {
    if (yValue > 0) {
      input = 1;
    } else {
      input = 3;
    }
  }
  return input;
}

void showLight(int light, int ledValue) {
  switch (light) {
  case 1:
    digitalWrite(yellowLED, ledValue);
    break;
  case 2:
    digitalWrite(redLED, ledValue);
    break;
  case 3:
    digitalWrite(blueLED, ledValue);
    break;
  case 4:
    digitalWrite(greenLED, ledValue);
    break;
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  waitForInput();
  playGame();
}

