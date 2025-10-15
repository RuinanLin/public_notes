# Example 2 - Using `map_async`

from multiprocessing import Pool
import time

def cube(x):
    time.sleep(1)
    return x ** 3

if __name__ == "__main__":
    with Pool(4) as pool:
        async_result = pool.map_async(cube, range(6))
        print("Doing other work in the main process...")
        results = async_result.get()    # Waits for results
        print("Results:", results)
