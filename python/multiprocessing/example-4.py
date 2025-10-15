# Example 4 - Computing Fibonacci

import time
from multiprocessing import Pool

def fib(n):
    if n <= 1:
        return n
    return fib(n-1) + fib(n-2)

def serial_run():
    start = time.time()
    results = [fib(35) for _ in range(6)]
    print("Serial time:", time.time() - start)

def parallel_run():
    start = time.time()
    with Pool(6) as pool:
        results = pool.map(fib, [35]*6)
    print("Parallel time:", time.time() - start)

if __name__ == "__main__":
    serial_run()
    parallel_run()
