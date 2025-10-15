# Example 3 - Using `starmap`

from multiprocessing import Pool

def power(base, exp):
    return base ** exp

if __name__ == "__main__":
    data = [(2, 3), (3, 2), (10, 3)]
    with Pool(3) as pool:
        results = pool.starmap(power, data)
    print("Results:", results)
