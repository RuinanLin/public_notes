# Example 6 - Pickling NumPy arrays

import pickle
import numpy as np

arr = np.arange(1e6)

with open('array.pkl', 'wb') as f:
    pickle.dump(arr, f, protocol=5)

with open('array.pkl', 'rb') as f:
    restored = pickle.load(f)

print(np.array_equal(arr, restored))    # True
