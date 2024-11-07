#include <Arduino.h>
// Pinouts //
#define yellowLED 4
#define redLED 0
#define blueLED 2
#define greenLED 15
#define vrxPin 1
#define vryPin 3
#define joystickButton 6
#define inputThreshold 400

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // for pseudo random function
}

void loop() {
  // waitForInput();
  // playGame();
}

bool waitForInput() {
  int xValue = analogRead(vrxPin);
  int yValue = analogRead(vryPin);

  // filter out noise, check for input
  while ((abs(xValue) <  inputThreshold) || (abs(yValue) < inputThreshold)) {
    delay(1);
  }
  return true;
}

void showLight(int light, int ledValue) {
  // display the correct light
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
  // turn on and off all lights twice
  int fps = 2; // flashes per second
  int delayAmount = 1000/(fps*2);
  for (int i=1;2;i++) {
    for (int light=1;4;light++) {
      showLight(light, HIGH);
    }
    delay(delayAmount);
    for (int light=1;4;light++) {
      showLight(light, LOW);
    }
    delay(delayAmount);
  }
}

int mattsCookedArrayAppend(int array, int nextValue) {
  array = array * 10 + nextValue; // shift all numbers to the next 10's place, add the new number in the 1's palce.
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
    next = mattsCookedArrayIndexReturn(sequence, i);
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
  int currentTime = millis();
  int stopTime = currentTime + 5000;
  while (currentTime < stopTime) {
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
