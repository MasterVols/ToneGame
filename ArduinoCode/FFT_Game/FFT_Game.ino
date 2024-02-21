// Matthew Bringle and Edward Tan
// FFT_Game

#include "arduinoFFT.h"

#define CHANNEL A0
#define BUTTON 12

arduinoFFT FFT;
const uint16_t samples = 512; // Must be a power of 2
const double samplingFrequency = 1500; // Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[samples];
double vImag[samples];

bool GameStarted = false;
int ticker = 0;
bool isLeft = true;

int cathodePins[] = {14, 15, 16, 17, 18, 19, 20, 21};
//int cathodePins[] = {21, 20, 19, 18, 17, 16, 15, 14};
//int anodePins[] = {9, 8, 7, 6, 5, 4, 3, 2};
int anodePins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int ledMatrix[8][8];

int frown[8][8] = {
          {0, 1, 0, 0, 0, 0, 1, 0},
          {0, 1, 0, 0, 0, 0, 1, 0},
          {0, 1, 1, 0, 0, 1, 1, 0},
          {0, 0, 1, 1, 1, 1, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 1, 0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0, 1, 0, 0}
        };

int smiley[8][8] = {
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 0, 0, 0, 0, 1, 0},
          {1, 1, 0, 0, 0, 0, 1, 1},
          {1, 0, 0, 0, 0, 0, 0, 1},
          {0, 0, 1, 0, 0, 1, 0, 0},
          {0, 1, 1, 0, 0, 1, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0}
        };

int left[8][8] = {
          {1, 0, 1, 0, 0, 0, 0, 0},
          {0, 1, 0, 1, 0, 0, 0, 0},
          {1, 0, 1, 0, 0, 0, 0, 0},
          {0, 1, 0, 1, 0, 0, 0, 0},
          {1, 0, 1, 0, 0, 0, 0, 0},
          {0, 1, 0, 1, 0, 0, 0, 0},
          {1, 0, 1, 0, 0, 0, 0, 0},
          {0, 1, 0, 1, 0, 0, 0, 0}
        };

int right[8][8] = {
          {0, 0, 0, 0, 0, 1, 0, 1},
          {0, 0, 0, 0, 1, 0, 1, 0},
          {0, 0, 0, 0, 0, 1, 0, 1},
          {0, 0, 0, 0, 1, 0, 1, 0},
          {0, 0, 0, 0, 0, 1, 0, 1},
          {0, 0, 0, 0, 1, 0, 1, 0},
          {0, 0, 0, 0, 0, 1, 0, 1},
          {0, 0, 0, 0, 1, 0, 1, 0}
      };

void displayPattern(int pattern[8][8]) {
  // Copy the pattern to the ledMatrix
  for (int col = 0; col < 8; col++) {
    for (int row = 0; row < 8; row++) {
      ledMatrix[row][col] = pattern[row][col];
    }
  }

  // Display the pattern on the LED matrix
  for (int i = 0; i < 256; i++) {
    for (int col = 0; col < 8; col++) {
      for (int row = 0; row < 8; row++) {
        digitalWrite(cathodePins[row], ledMatrix[row][col] ? HIGH : LOW);
      }
      digitalWrite(anodePins[col], LOW);
      delayMicroseconds(1000);
      digitalWrite(anodePins[col], HIGH);

      for (int row = 0; row < 8; row++) {
        digitalWrite(cathodePins[row], LOW);
      }
      delayMicroseconds(50);
    }
  }
}

///////////////////////////////////////////////////////////////////////////

void setup() {
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Ready");

  for (int i = 0; i < 8; i++) {
    pinMode(cathodePins[i], OUTPUT);
    pinMode(anodePins[i], OUTPUT);
  }

  pinMode(BUTTON, INPUT);
}

void loop() {

  if (!GameStarted) {

    if (digitalRead(BUTTON)) {
      GameStarted = true;  
    }
    
  } else {

    

    

    if (ticker > 2) {
      // Check win conditions
      int leftScore = 0;
      int rightScore = 0;
    
      // Sum the heights of the LEDs in the first four columns for the left score
      for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 8; row++) {
          leftScore += ledMatrix[row][col];
        }
      }
    
      // Sum the heights of the LEDs in the last four columns for the right score
      for (int col = 4; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
          rightScore += ledMatrix[row][col];
        }
      }
    
      // Check if the side matches and display a smiley or frown face accordingly
      if ((isLeft && leftScore > rightScore) || (!isLeft && rightScore > leftScore)) {
        // Display smiley face

        displayPattern(smiley);
      } else {
        // Display frown face
        
        displayPattern(frown);
      }

      ticker = 0;
    }

    if (ticker == 0) {
      // Randomize if the game is going to be left or right
      isLeft = random(0, 2); // Returns 0 or 1 randomly
    
      // Define isLeft here and show the "left" or "right" matrix to the user
      if (isLeft) {
        displayPattern(left);
      } else {
        // Define the "right" pattern similar to the "left" pattern but reversed
        displayPattern(right);
      }
    }

    ticker++;

    // Sampling
    microseconds = micros();
    for (int i = 0; i < samples; i++) {
      vReal[i] = analogRead(CHANNEL);
      vImag[i] = 0;
      while (micros() - microseconds < sampling_period_us) {
        // Empty loop
      }
      microseconds += sampling_period_us;
    }
    FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();
    vReal[0] = 0;
    vReal[1] = 100;
  
    Serial.println("FFT Magnitudes for averaged frequency bins:");
    // Map FFT magnitudes to LED matrix
    for (int col = 0; col < 8; col++) {
      // Calculate the average magnitude for a set of frequency bins
      double sum = 0;
      int binSize = samples / 64; // Number of bins to average for each column
      for (int i = 0; i < binSize; i++) {
        int index = 1 + col * binSize + i; // Skip the first bin (0 Hz) and adjust index
        sum += vReal[index];
      }
      double avgMagnitude = sum / binSize;
      int height = map(avgMagnitude, 0, 800, 1, 8); // Map average magnitude to LED column height
      Serial.print("Average Magnitude for Column ");
      Serial.print(col);
      Serial.print(": ");
      Serial.print(avgMagnitude);
      Serial.print(" With Height: ");
      Serial.println(height);
  
      //WRITE FREQS TO MATRIX
      for (int row = 0; row < 8; row++) {
        ledMatrix[row][col] = (row < height) ? 1 : 0;
      }
    }
  
    for (int i = 0; i < 128; i++) {
      // Display the pattern on the LED matrix
      for (int col = 0; col < 8; col++) {
        for (int row = 8; row > 0; row--) {
          digitalWrite(cathodePins[row], ledMatrix[row][col] ? HIGH : LOW);
        }
        digitalWrite(anodePins[col], LOW);
        delayMicroseconds(1000);
        digitalWrite(anodePins[col], HIGH);
  
        for (int row = 0; row < 8; row++) {
          digitalWrite(cathodePins[row], LOW);
        }
        delayMicroseconds(50);
      }
    }
    delay(50);

    if (digitalRead(BUTTON)) {
      GameStarted = false;  
      delay(1000);
    }
  }
  
}
