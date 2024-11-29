import time
import numpy as np
import serial


def send_matrix(ser, matrix):
    for value in matrix.flatten():
        ser.write(bytes([value + 128]))


def receive_matrix(ser, rows, cols):
    matrix = np.zeros((rows, cols), dtype=np.int16)
    for i in range(rows):
        for j in range(cols):
            value_str = ser.readline().decode().strip()
            matrix[i, j] = int(value_str)
    return matrix


ser = serial.Serial("/dev/tty.usbmodem2101", 9600, timeout=5)
time.sleep(2)

print("Connected...")

while True:
    rows_A = int(input("A matrix rows (1-16, or 0 to quit): "))
    cols_A = int(input("A matrix columns (1-16): "))
    cols_B = int(input("B matrix columns (1-16): "))

    if rows_A == 0:
        break

    A = np.random.randint(-128, 127, (rows_A, cols_A), dtype=np.int8)
    B = np.random.randint(-128, 127, (cols_A, cols_B), dtype=np.int8)

    print("Matrix A:")
    print(A)
    print("Matrix B:")
    print(B)

    # Send size and matrices
    ser.write(bytes([rows_A]))
    ser.write(bytes([cols_A]))
    ser.write(bytes([cols_B]))
    send_matrix(ser, A)
    send_matrix(ser, B)

    C = receive_matrix(ser, rows_A, cols_B)

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
