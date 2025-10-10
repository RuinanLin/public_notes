# Example 3 - Custom class

import pickle

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __repr__(self):
        return f"Point({self.x}, {self.y})"

p = Point(3, 4)

# Serialize
with open('point.pkl', 'wb') as f:
    pickle.dump(p, f)

# Deserialize
with open('point.pkl', 'rb') as f:
    restored = pickle.load(f)

print(restored) # Point(3, 4)
print(isinstance(restored, Point))  # True
