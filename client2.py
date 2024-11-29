import time
import numpy as np
import serial


def send_matrix(ser, matrix):
    for value in matrix.flatten():
        ser.write(bytes([value + 128]))


def receive_matrix(ser, size):
    matrix = np.zeros((size, size), dtype=np.int16)
    for i in range(size):
        for j in range(size):
            value_str = ser.readline().decode().strip()
            matrix[i, j] = int(value_str)
    return matrix


ser = serial.Serial("/dev/tty.usbmodem2101", 9600, timeout=5)
time.sleep(2)

print("Connected...")

while True:
    size = int(input("Matrix size (1-16, or 0 to quit): "))
    if size == 0:
        break
    if size < 1 or size > 16:
        print("Invalid size. Please enter a size between 1 and 16.")
        continue

    A = np.random.randint(-128, 127, (size, size), dtype=np.int8)
    B = np.random.randint(-128, 127, (size, size), dtype=np.int8)

    print("Matrix A:")
    print(A)
    print("Matrix B:")
    print(B)

    # Send size and matrices
    ser.write(bytes([size]))
    send_matrix(ser, A)
    send_matrix(ser, B)

    C = receive_matrix(ser, size)

    print("Result Matrix C:")
    print(C)

    expected = np.matmul(A.astype(np.int32), B.astype(np.int32)).astype(np.int16)
    print(f"Expected Matrix C: {expected}")
    if np.array_equal(C, expected):
        print("Result passed.")
    else:
        print("Result failed")
        print("Expected:")
        print(expected)
        print("Difference:")
        print(C - expected)

ser.close()
print("Connection closed.")
