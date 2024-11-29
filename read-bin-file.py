import struct


def read_first_number(filename, dtype="int8"):
    with open(filename, "rb") as f:
        if dtype == "int8":
            number = struct.unpack("b", f.read(1))[0]
        elif dtype == "int32":
            number = struct.unpack("i", f.read(4))[0]
        else:
            raise ValueError("Unsupported dtype. Use 'int8' or 'int32'.")
    return number


# Replace 'q.bin' with the actual path to your file if it's not in the same directory
filename = "/Users/ryan/Documents/gpt/embed_seq.bin"

# Assuming the file contains int8 values. Change to 'int32' if necessary.
first_number = read_first_number(filename, dtype="int8")

print(f"The first number in {filename} is: {first_number}")
