# Example 1 - Serialize and Deserialize a Python Object

import pickle

data = {
    'name': 'Alice',
    'age': 25,
    'scores': [95, 87, 92],
    'active': True
}

# Serialize to bytes
serialized = pickle.dumps(data)
print(serialized)   # b'\x80\x04\x95...'
print(type(serialized)) # <class 'bytes'>

# Deserialize from bytes
restored = pickle.loads(serialized)
print(restored)
# {'name': 'Alice', 'age': 25, 'scores': [95, 87, 92], 'active': True}
