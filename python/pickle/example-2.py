# Example 2 - Using files directly

import pickle

data = {'x': 10, 'y': 20}

# Serialize (write to file)
with open('data.pkl', 'wb') as f:
    pickle.dump(data, f)

# Deserialize (read from file)
with open('data.pkl', 'rb') as f:
    loaded = pickle.load(f)

print(loaded)   # {'x': 10, 'y': 20}
