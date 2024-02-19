// Define the pins for the cathodes and anodes
int cathodePins[] = {14, 15, 16, 17, 18, 19, 20, 21};
int anodePins[] = {9, 8, 7, 6, 5, 4, 3, 2};

// Define the 2D array for the LED matrix
// Use 1 for an LED that should be on and 0 for an LED that should be off
int ledMatrix[8][8] = {
  {1, 0, 1, 1, 1, 1, 1, 1},
  {0, 1, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

void setup() {
  // Set all cathode and anode pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(cathodePins[i], OUTPUT);
    pinMode(anodePins[i], OUTPUT);
  }
}

void loop() {
  // Display the pattern defined in the ledMatrix array, one column at a time
  for (int col = 0; col < 8; col++) {
    for (int row = 0; row < 8; row++) {
      if (ledMatrix[row][col] == 1) {
        digitalWrite(cathodePins[row], HIGH); // Turn on the LED
      } else {
        digitalWrite(cathodePins[row], LOW); // Turn off the LED
      }
    }
    digitalWrite(anodePins[col], LOW); // Enable the current column
    delayMicroseconds(1000); // Adjust the delay for brightness and smoothness
    digitalWrite(anodePins[col], HIGH); // Disable the current column before moving to the next one

    // Turn off all LEDs in the row before moving to the next column
    for (int row = 0; row < 8; row++) {
      digitalWrite(cathodePins[row], LOW);
    }

    delayMicroseconds(50);
  }
}
