# Example 1 - Basic Usage Example

from multiprocessing import Pool
import time

def square(n):
    print(f"Processing {n}")
    time.sleep(1)   # Simulate expensive work
    return n * n

if __name__ == "__main__":
    numbers = [1, 2, 3, 4, 5]

    # Create a pool with 4 worker processes
    with Pool(processes=4) as pool:
        results = pool.map(square, numbers)

    print("Results:", results)
