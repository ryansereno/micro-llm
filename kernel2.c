// #include <avr/io.h>
#include <stdint.h>
// #include <util/delay.h>

#define BAUD 9600
#define F_CPU 16000000UL
#define UBRR_VALUE ((F_CPU / (BAUD * 16UL)) - 1)
#define MAX_SIZE 16 // Maximum matrix size

// UART serial communication registers
#define UBRR0H (*((volatile uint8_t *)0xC5))
#define UBRR0L (*((volatile uint8_t *)0xC4))
#define UCSR0A (*((volatile uint8_t *)0xC0))
#define UCSR0B (*((volatile uint8_t *)0xC1))
#define UCSR0C (*((volatile uint8_t *)0xC2))
#define UDR0 (*((volatile uint8_t *)0xC6))

// UCSR0B bits
#define RXEN0 4
#define TXEN0 3

// UCSR0C bits
#define UCSZ00 1

// UCSR0A bits
#define RXC0 7
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

void uart_print_number(int16_t num) {
  char buffer[7];
  itoa(num, buffer, 10);
  for (char *p = buffer; *p; p++) {
    uart_transmit(*p);
  }
  uart_transmit('\n'); // Send newline
}

void matrix_multiply(int8_t *A, int8_t *B, int16_t *C, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    for (uint8_t j = 0; j < size; j++) {
      int32_t sum = 0;
      for (uint8_t k = 0; k < size; k++) {
        sum += (int32_t)A[i * size + k] * B[k * size + j];
      }
      C[i * size + j] = (int16_t)sum;
    }
  }
}

int main(void) {
  uart_init();

  while (1) {
    // Receive matrix size
    uint8_t size = uart_receive();
    if (size > MAX_SIZE) {
      uart_print_number(-1); // Error: size too large
      continue;
    }

    // Receive matrices A and B
    int8_t A[MAX_SIZE * MAX_SIZE], B[MAX_SIZE * MAX_SIZE];
    int16_t C[MAX_SIZE * MAX_SIZE];
    for (uint16_t i = 0; i < size * size; i++) {
      A[i] = (int8_t)uart_receive() - 128;
    }
    for (uint16_t i = 0; i < size * size; i++) {
      B[i] = (int8_t)uart_receive() - 128;
    }

    // Multiply matrices
    matrix_multiply(A, B, C, size);

    // Send result matrix C
    for (uint16_t i = 0; i < size * size; i++) {
      uart_print_number(C[i]);
    }
  }

  return 0;
}
