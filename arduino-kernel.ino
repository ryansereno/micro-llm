#include <SD.h>

#define CS_PIN 4
#define MATRIX_SIZE 768
#define SEQUENCE_LENGTH 7

void matrix_multiply(File &A, File &B, File &C, uint16_t rows_A, uint16_t cols_A, uint16_t cols_B) {
  int8_t a_val, b_val;
  int32_t product;
  File temp = SD.open("temp.bin", FILE_WRITE);
  
  if (!temp) {
    Serial.println("Error opening temp file!");
    return;
  }

  for (uint16_t i = 0; i < rows_A; i++) {
    for (uint16_t j = 0; j < cols_B; j++) {
    	Serial.print("Calculating element ");
      int32_t sum = 0;
      for (uint16_t k = 0; k < cols_A; k++) {
        A.seek((i * cols_A + k) * sizeof(int8_t));
        B.seek((k * cols_B + j) * sizeof(int8_t));
        a_val = A.read();
        b_val = B.read();
        product = (int32_t)a_val * b_val;
        sum += product;
      }
      temp.write((uint8_t *)&sum, sizeof(int32_t));
    }
  }
  
  temp.close();
  
  // Copy result from temp file to C
  temp = SD.open("temp.bin", FILE_READ);
  if (!temp) {
    Serial.println("Error opening temp file for reading!");
    return;
  }
  
  while (temp.available()) {
    int32_t value;
    temp.read((uint8_t *)&value, sizeof(int32_t));
    C.write((uint8_t *)&value, sizeof(int32_t));
  }
  
  temp.close();
  SD.remove("temp.bin");
}

void multiply_matrices() {
  File A = SD.open("matrixA.bin", FILE_READ);
  File B = SD.open("matrixB.bin", FILE_READ);
  File C = SD.open("result.bin", FILE_WRITE);
  
  if (!A || !B || !C) {
    Serial.println("Error opening matrix files!");
    return;
  }
  
  matrix_multiply(A, B, C, MATRIX_SIZE, MATRIX_SIZE, SEQUENCE_LENGTH);
  
  A.close();
  B.close();
  C.close();
  
  Serial.println("Matrix multiplication complete!");
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  multiply_matrices();
}

void loop() {
  // Nothing to do here
}
