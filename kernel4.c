#include <SD.h>
#include <SPI.h>
#include <stdint.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (BAUD * 16UL)) - 1)

#define CS_PIN 4

// UART Registers
#define UBRR0H (*((volatile uint8_t *)0xC5))
#define UBRR0L (*((volatile uint8_t *)0xC4))
#define UCSR0A (*((volatile uint8_t *)0xC0))
#define UCSR0B (*((volatile uint8_t *)0xC1))
#define UCSR0C (*((volatile uint8_t *)0xC2))
#define UDR0 (*((volatile uint8_t *)0xC6))

// UART Bits
#define RXEN0 4
#define TXEN0 3
#define UCSZ00 1
#define UDRE0 5

void uart_init() {
  UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
  UBRR0L = (uint8_t)UBRR_VALUE;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (3 << UCSZ00);
}

void uart_transmit(uint8_t data) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}

uint8_t uart_receive() {
  while (!(UCSR0A & (1 << RXC0)))
    ;
  return UDR0;
}

// send string over uart
void uart_print_number(int16_t num) {

  // Longest 16-bit signed integer is -32768
  // 6 chars for the digits and sign
  // 1 character for the null terminator ('\0')
  char buffer[7];

  // convert base 10 number to a string and place in buffer
  itoa(num, buffer, 10);

  for (char *p = buffer; *p; p++) {
    uart_transmit(*p);
  }
  uart_transmit('\n');
}

void matrix_multiply(File *A, File *B, File *C, uint16_t rows_A,
                     uint16_t cols_A, uint16_t cols_B) {
  int8_t a_val, b_val;
  int32_t product;

  File temp = SD.open("temp.bin", FILE_WRITE);

  for (uint16_t i = 0; i < rows_A; i++) {
    for (uint16_t j = 0; j < cols_B; j++) {
      for (uint16_t k = 0; k < cols_A; k++) {

        A->seek((i * cols_A + k) * sizeof(int8_t));
        B->seek((k * cols_B + j) * sizeof(int8_t));

        A->read(&a_val, sizeof(int8_t));
        B->read(&b_val, sizeof(int8_t));

        product =
            (int32_t)a_val *
            b_val; // cast to int32 BEFORE multiplication, to avoid overflow
        temp.write((uint8_t *)&product, sizeof(int32_t));
      }
    }
  }
}

int main(void) {
  uart_init();

  if (!SD.begin(CS_PIN)) {
    while (1)
      ;
  }

  multiply_matrices();

  while (1) {
    // Main program loop
  }

  return 0;
}
