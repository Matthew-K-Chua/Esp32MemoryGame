#include <Arduino.h>
static int yellowLED = 4;
static int redLED = 0;
static int blueLED = 2;
static int greenLED = 15;
const int vrxPin = 1;
const int vryPin = 3;
#define inputThreshold 400

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  // waitForInput();
  // playGame();
}

bool waitForInput() {
  int xValue = analogRead(vrxPin);
  int yValue = analogRead(vryPin);
  while ((abs(xValue) <  inputThreshold) || (abs(yValue) > inputThreshold)) {
    delay(1);
  }
  return true;
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
  if (sequence > 0) {
    sequence = mattsCookedArrayAppend(sequence, nextInt); // "append next in sequence"
  } else {
    sequence = nextInt; // if the game has only begun, set sequence to the first rand int
  }
  return sequence;
}

int mattsCookedArrayIndexReturn(int array, int index) {
    int arrayLength = floor(log(array)/log(10)); // get the length of the array
    int factor = pow(10, ((arrayLength - index + 1))); // find the multiplier that will put the index in the 1's place
    int arrayChunk = floor(array/factor); // use that multiplier to shrink the number
    int indexValue = arrayChunk%10; // get the 1's place
    return indexValue;
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

int getPlayerInput() { // verify that this outputs the correct things, otherwise change the numbers
  int input = 0;
  while (!waitForInput()) {
    delay(1);
  }
  int xValue = analogRead(vrxPin);
  int yValue = analogRead(vryPin);
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

bool playerTurn(int sequence) {
  int nextLot = 0;
  int next = 0;
  int factor = 0;
  int input = 0;
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
  return roundWon;
}

void playGame() {
  int sequence = 0;
  bool roundWon = true;
  while (roundWon == true)
    flashLights();
    sequence = createSequence(sequence);
    displaySequence(sequence);
    flashLights();
    roundWon = playerTurn(sequence);
}

// TESTS //

void testHardware() {
  // test LEDS on and off
  // test the joystick
  // test the joystick button
}

void waitForInputTest() {
  // add a printout saying start, don't move
  // add a printout saying move now
  // add a printout after input is received
}

void showLightTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING showLight");
  int ledValue = HIGH;
  digitalWrite(yellowLED, ledValue);
  Serial.println("Yellow On");
  digitalWrite(redLED, ledValue);
  Serial.println("Red On");
  digitalWrite(blueLED, ledValue);
  Serial.println("Blue On");
  digitalWrite(greenLED, ledValue);
  Serial.println("Green On");
  delay(500);
  ledValue = LOW;
  digitalWrite(yellowLED, ledValue);
  digitalWrite(redLED, ledValue);
  digitalWrite(blueLED, ledValue);
  digitalWrite(greenLED, ledValue);
  Serial.println("Test Complete");
}

void flashLightsTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING flashLights");
  flashLights();
  Serial.println("Test Complete");
}


void mattsCookedArrayAppendTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING mattsCookedArrayAppend");
  int array = 1234;
  int nextValue = 5;
  int arrayLength = floor(log(array)/log(10)); // get the length of the array
  array += pow(10,arrayLength+1)*nextValue;
  if (array == 12345) {
    Serial.println("mattsCookedArrayAppend passed");
  } else {
    Serial.println("mattsCookedArrayAppend failed");
  }
  Serial.println("Test Complete");
}

void createSequenceTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING createSequence");
  int sequence = 1234;
  int nextInt = random(4); // get a random 1/2/3/4
  if (sequence > 0) {
    sequence = mattsCookedArrayAppend(sequence, nextInt); // "append next in sequence"
  } else {
    sequence = nextInt; // if the game has only begun, set sequence to the first rand int
  }
  if (sequence >= 12341 || sequence <= 12344) {
    Serial.println("createSequence passed");
  }
  Serial.println("Test Complete");
}

void mattsCookedArrayIndexReturnTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING mattsCookedArrayIndexReturn");
  int testArray = 123456789;
  int indexOne = 1;
  int indexTwo = 2;
  int indexNine = 9;
  int totalPass = 0;
  int valueAtIndexOne = mattsCookedArrayIndexReturn(testArray, indexOne);
  int valueAtIndexTwo = mattsCookedArrayIndexReturn(testArray, indexTwo);
  int valueAtIndexNine = mattsCookedArrayIndexReturn(testArray, indexNine);
  if (valueAtIndexOne == indexOne) {
    Serial.println("indexOne passed");
    totalPass++;
  } else {
    Serial.println("indexOne failed");
  }

  if (valueAtIndexTwo == indexTwo) {
    Serial.println("indexTwo passed");
    totalPass++;
  } else {
    Serial.println("indexTwo failed");
  }

  if (valueAtIndexNine == indexNine) {
    Serial.println("indexNine passed");
    totalPass++;
  } else {
    Serial.println("indexNine failed");
  }

  if (totalPass == 3) {
    Serial.println("mattsCookedArrayIndexReturn passed");
  }
  Serial.println("Test Complete");
}

void displaySequenceTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING displaySequence");
  int sequence = 1234;
  int next;
  int sequenceLength = floor(log(sequence)); // get the power of 10 that the sequence is up to
  for (int i=1;sequenceLength;i++) {
    next = mattsCookedArrayIndexReturn(sequence, i);
    showLight(next, HIGH);
    delay(500);
    showLight(next, LOW);
  }
  Serial.println("Test Complete");
}

void getPlayerInputTest() { // each input will illuminate the correct bulb for 5 seconds
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING getPlayerInput");
  int currentTime = millis();
  int stopTime = currentTime + 5000;
  while (currentTime < stopTime) {
    int input = getPlayerInput();
    showLight(input, HIGH);
    delay(100);
    showLight(input, LOW);
  }
  Serial.println("Test Complete");
}