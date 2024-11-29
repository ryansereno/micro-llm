import serial
import time


def send_numbers(ser, a, b):
    ser.write(bytes([a + 128, b + 128]))


def receive_result(ser):
    return int(ser.readline().decode().strip())


# Open serial connection
ser = serial.Serial("/dev/tty.usbmodem2101", 9600, timeout=1)
time.sleep(2)

print("Connected to Arduino. Ready...")

while True:
    try:
        a = int(input("Enter first number (-128 to 127): "))
        if not -128 <= a <= 127:
            raise ValueError("Number must be between -128 and 127")

        b = int(input("Enter second number (-128 to 127): "))
        if not -128 <= b <= 127:
            raise ValueError("Number must be between -128 and 127")

        print(f"\nMultiplying {a} and {b}")

        # Send numbers to Arduino
        send_numbers(ser, a, b)

        # Receive result from Arduino
        arduino_result = receive_result(ser)

        # Calculate expected result
        expected_result = a * b

        print(f"Arduino result: {arduino_result}")
        print(f"Expected result: {expected_result}")

        if arduino_result == expected_result:
            print("Result passed.")
        else:
            print("Result failed.")

        cont = input("Continue? (y/n): ").lower()
        if cont != "y":
            break

    except ValueError as e:
        print(f"Invalid input: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")
        break

ser.close()
print("Connection closed.")
