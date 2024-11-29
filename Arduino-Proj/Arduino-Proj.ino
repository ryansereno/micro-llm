#include <SD.h>

#define CS_PIN 4
#define MATRIX_SIZE 768
#define SEQUENCE_LENGTH 7

void mat_mul(File &A, File &B, File &C, uint16_t rows_A, uint16_t cols_A, uint16_t cols_B) {
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
	Serial.print(a_val);
	Serial.print(b_val);
	Serial.print(product);
        sum += product;
      }
      temp.write((uint8_t *)&sum, sizeof(int32_t));
      temp.flush();
    }
  }
  
  temp.close();
  
}

void q_k_v() {
  File Qw = SD.open("Qw.BIN", FILE_READ);
  File Embed = SD.open("EMBED.BIN", FILE_READ);
  File Q = SD.open("Q.BIN", FILE_WRITE);
  
  if (!Qw || !Embed || !Q) {
    Serial.println("Error opening matrix files!");
    return;
  }
  
  mat_mul(Qw, Embed, Q, MATRIX_SIZE, MATRIX_SIZE, SEQUENCE_LENGTH);
  
  Qw.close();
  Embed.close();
  Q.close();

  File Kw = SD.open("Kw.BIN", FILE_READ);
  File Embed = SD.open("EMBED.BIN", FILE_READ);
  File K = SD.open("K.BIN", FILE_WRITE);
  
  mat_mul(Kw, Embed, K, MATRIX_SIZE, MATRIX_SIZE, SEQUENCE_LENGTH);
  
  Kw.close();
  Embed.close();
  K.close();

  File Vw = SD.open("Vw.BIN", FILE_READ);
  File Embed = SD.open("EMBED.BIN", FILE_READ);
  File V = SD.open("V.BIN", FILE_WRITE);

  mat_mul(Vw, Embed, V, MATRIX_SIZE, MATRIX_SIZE, SEQUENCE_LENGTH);

  Vw.close();
  Embed.close();
  V.close();
  
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
  
  q_k_v();
}

void loop() {
  // Nothing to do here
}


