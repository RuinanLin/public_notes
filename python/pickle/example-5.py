# Example 5 - Pickling multiple objects

import pickle

with open('multi.pkl', 'wb') as f:
    pickle.dump([1, 2, 3], f)
    pickle.dump({'a': 10}, f)

# You must load them in the same order
with open('multi.pkl', 'rb') as f:
    a = pickle.load(f)
    b = pickle.load(f)

print(a, b) # [1, 2, 3] {'a': 10}
