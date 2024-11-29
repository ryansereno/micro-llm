#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define BAUD 9600
#define F_CPU 16000000UL
#define UBRR_VALUE ((F_CPU / (BAUD * 16UL)) - 1)

#define MAX_SIZE 16 // Maximum matrix size

// store matrices as 1D arrays
int8_t A[MAX_SIZE * MAX_SIZE];
int8_t B[MAX_SIZE * MAX_SIZE];
int8_t C[MAX_SIZE * MAX_SIZE];
uint8_t size = 0;

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

void uart_print_string(const char *str) {
  while (*str) {
    uart_transmit(*str++);
  }
}
void uart_print_number(int8_t number) {
  char buffer[5];
  itoa(number, buffer, 10);
  uart_print_string(buffer);
}

void matrix_multiply(int8_t *A, int8_t *B, int8_t *C, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    for (uint8_t j = 0; j < size; j++) {
      int16_t sum = 0;
      for (uint8_t k = 0; k < size; k++) {
        int16_t product = (int16_t)A[i * size + k] * B[k * size + j];
        sum += product;
        uart_print_string("Prod: ");
        uart_print_number(product);
        uart_print_string(" Sum: ");
        uart_print_number(sum);
        uart_print_string("\r\n");
      }
      C[i * size + j] = (int8_t)sum;
      uart_print_string("Final C: ");
      uart_print_number(C[i * size + j]);
      uart_print_string("\r\n");
    }
  }
}

int main(void) {
  uart_init();
  uart_print_string("Ready for matrix input\r\n");

  while (1) {
    if (UCSR0A & (1 << RXC0)) { // Check if data is available
      size = uart_receive();
      if (size > MAX_SIZE) {
        uart_print_string("Size too large\r\n");
        continue;
      }

      // Receive matrix A
      for (uint8_t i = 0; i < size * size; i++) {
        A[i] = (int8_t)uart_receive() - 128;
      }

      // Receive matrix B
      for (uint8_t i = 0; i < size * size; i++) {
        B[i] = (int8_t)uart_receive() - 128;
      }

      matrix_multiply(A, B, C, size);

      // Send result matrix C
      for (uint8_t i = 0; i < size * size; i++) {
        uart_transmit(C[i]);
      }
    }
  }

  return 0;
}
