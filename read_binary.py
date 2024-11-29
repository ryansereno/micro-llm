import struct
import numpy as np
def read_binary_file(filename, dtype=np.int32):
    with open(filename, 'rb') as f:
        return np.frombuffer(f.read(), dtype=dtype)
# Read the data
data = read_binary_file('TEMP.BIN')
# Print the first few elements
print("First 10 elements:")
print(data[:10])
# Print some statistics
print("\nStatistics:")
print(f"Number of elements: {len(data)}")
print(f"Min value: {data.min()}")
print(f"Max value: {data.max()}")
print(f"Mean value: {data.mean()}")
