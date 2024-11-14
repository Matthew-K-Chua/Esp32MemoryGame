#include <Arduino.h>
// Pinouts //
// need to actually confirm these pins are valid.
#define yellowLED 16
#define redLED 17
#define blueLED 5
#define greenLED 18
#define vrxPin 15
#define vryPin 2
#define joystickButton 4

// Thresholds for the joystick
#define inputThresholdSmall 550
#define inputThresholdBig 3500

bool waitForInput() {
  // waits for user input then returns back
  float xValue = analogRead(vrxPin);
  float yValue = analogRead(vryPin);

  // filter out noise, check for input
  Serial.println("Waiting for player input");
  while ((xValue >  inputThresholdSmall) && (xValue < inputThresholdBig) && (yValue > inputThresholdSmall) && (yValue < inputThresholdBig) && (digitalRead(joystickButton) == LOW)) {
    xValue = analogRead(vrxPin);
    yValue = analogRead(vryPin);
    delay(10);
  }
  Serial.println(xValue);
  Serial.println(yValue);
  return true;
}

void showLight(int light, int ledValue) {
  // display the correct light as per the diagram in obsidian
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
  default:
    break;
  }
}

void flashLights() {
  // turn on and off all lights twice
  int fps = 4; // flashes per second
  int delayAmount = floor(1000/(fps*2));
  for (int i = 1; i <= 2; i++) {
    for (int light = 1; light <= 4; light++) {
      showLight(light, HIGH);
    }
    delay(delayAmount);
    for (int light = 1; light <= 4; light++) {
      showLight(light, LOW);
    }
    delay(delayAmount);
  }
}

int mattsCookedArrayAppend(int array, int nextValue) {
  // appends another value to this memory wasting array
  // This does work as intended
  array = array * 10 + nextValue; // shift all numbers to the next 10's place, add the new number in the 1's palce.
  return array;
}

int createSequence(int sequence) {
  // adds another step to the sequence
  int nextInt = random(1, 5); // get a random 1/2/3/4
  if (sequence > 0) {
    sequence = mattsCookedArrayAppend(sequence, nextInt); // "append next in sequence"
  } else {
    sequence = nextInt; // if the game has only begun, set sequence to the first rand int
  }
  return sequence;
}

int mattsCookedArrayIndexReturn(int array, int index) {
  // returns the index of this memory wasting array
  // This does work as intended
  int arrayLength = floor(log10(array)) + 1; // get the length of the array
  int factor = pow(10, ((arrayLength - index))); // find the multiplier that will put the index in the 1's place
  int arrayChunk = floor(array/factor); // use that multiplier to shrink the number
  int indexValue = arrayChunk%10; // get the 1's place
  return indexValue;
}

void displaySequence(int sequence) {
  // displays the sequence for a player to watch and remember
  int next;
  int fps = 1; // flashes per second
  int delayAmount = floor(1000/(fps*2));
  int sequenceLength = floor(log10(sequence)) + 1; // get the power of 10 that the sequence is up to
  for (int i = 1; i <= sequenceLength; i++) {
    next = mattsCookedArrayIndexReturn(sequence, i);
    showLight(next, HIGH);
    delay(delayAmount/2);
    showLight(next, LOW);
    if (i != sequenceLength) {
      delay(delayAmount/2);
    }
  }
}

int getPlayerInput() { // verify that this outputs the correct things, otherwise change the numbers
  // Waits for the player input and returns the corresponding value.
  int input = 0;
  waitForInput();
  int xValue = analogRead(vrxPin);
  int yValue = analogRead(vryPin);
  if ((yValue > inputThresholdSmall) && (yValue < inputThresholdBig)) {
    if (xValue > inputThresholdBig) {
      input = 3;
    } else {
      input = 1;
    }
  } else {
    if (yValue > inputThresholdBig) {
      input = 4;
    } else {
      input = 2;
    }
  }
  return input;
}

void victoryFlash() {
  // Flashes the lights in a cool sequence
  // when the player hits 9/9 (because then I hit integer overflow and I don't want my game to die like tetris)
  int fps = 8; // flashes per second
  int delayAmount = floor(1000/fps);
  // flash everything in a circle
  for (int i=1;i<=4;i++){
    for (int j=1;j<=4;j++) { // requires 5 passes to turn everything off
      showLight(j, HIGH);
      delay(delayAmount); 
      showLight(j, LOW);
    }
  }
  // then flash all four four times
  flashLights();
  flashLights();
}

void loseFlash() {
  flashLights();
  flashLights();
  flashLights();
  flashLights();
}

bool playerTurn(int sequence) {
  // Gets the player's input
  // Shines the corresponding light
  // Determines whether or not the player was correct
  int next = 0;
  int input = 0;
  bool roundWon = true;
  int sequenceLength = floor(log10(sequence)) + 1; // get the power of 10 that the sequence is up to
  for (int i = 1; i <= sequenceLength; i++) {
    Serial.println("waiting for input");
    input = getPlayerInput();
    Serial.print("Gotten input: ");
    Serial.println(input);
    showLight(input, HIGH);
    delay(500);
    showLight(input, LOW);
    next = mattsCookedArrayIndexReturn(sequence, i);
    Serial.println(next);
    if (input != next) {
      roundWon = false;
      return roundWon;
    }
  }
  flashLights();
  return roundWon;
}

void playGame() {
  // Plays the game until the player loses
  int sequence = 0;
  bool roundWon = true;
  flashLights();
  while (roundWon == true && sequence < 100000000) {
    Serial.println("starting... loop");
    sequence = createSequence(sequence);
    displaySequence(sequence);
    delay(500);
    flashLights();
    Serial.println(sequence);
    roundWon = playerTurn(sequence);
    Serial.println("finished... loop");
  }
  Serial.println("Exited loop, finished game");
  Serial.println(roundWon);
  Serial.println(sequence);
  if (roundWon == true) {
    victoryFlash();
  } else {
    loseFlash();
  }
}

// TESTS //
void waitForInputTest() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING waitForInput");
  waitForInput();
  Serial.println("Input received, test complete");
}

void testHardware() {
  Serial.println("/////////////////////////////////");
  Serial.println("TESTING Hardware");

  Serial.println("Testing LEDS");
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
  Serial.println("Finished testing LEDs");
  Serial.println("Testing joystick controller for 7 seconds. Please move the joystick and push the button");
  unsigned long stopTime = millis() + 5000;
  while (millis() < stopTime) {
    int xValue = analogRead(vrxPin);
    int yValue = analogRead(vryPin);
    int joystickButtonValue = analogRead(joystickButton);
    Serial.println(xValue);
    Serial.println(yValue);
    Serial.println(joystickButtonValue);
    delay(10);
  }
  Serial.println("Finished testing joystick controller.");
  Serial.println("Hardware test Complete");
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
  array = mattsCookedArrayAppend(array, nextValue);
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
  sequence = createSequence(sequence);
  if (sequence >= 12341 && sequence <= 12344) {
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
  int sequenceLength = floor(log10(sequence)); // get the power of 10 that the sequence is up to
  for (int i=1;i<sequenceLength;i++) {
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
  unsigned long stopTime = millis() + 5000;
  while (millis() < stopTime) {
    int input = getPlayerInput();
    showLight(input, HIGH);
    delay(100);
    showLight(input, LOW);
  }
  Serial.println("Test Complete");
}

// CODE //
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // for pseudo random function
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(vrxPin, INPUT);
  pinMode(vryPin, INPUT);
  pinMode(joystickButton, INPUT);
}

void loop() {
  // UNIT TESTS (passed) //
  // waitForInputTest(); // joystick works
  // testHardware(); // hardware works, not sure about joystick button tho
  // flashLightsTest(); // lights flash properly
  // mattsCookedArrayAppendTest();
  // createSequenceTest();
  // mattsCookedArrayIndexReturnTest();
  // displaySequenceTest();
  // getPlayerInputTest();

  waitForInput();
  playGame();
  // victoryFlash();

  delay(500);
}