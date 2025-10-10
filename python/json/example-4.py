# Example 4 - Using `default` parameter

# JSON only supports simple types.
# For custom classes or complex objects,
# you must provide custom serialization logic.

import json
from datetime import datetime

class User:
    def __init__(self, name, joined):
        self.name = name
        self.joined = joined

u = User("Alice", datetime(2024, 1, 15))

def encode_user(obj):
    if isinstance(obj, User):
        return {"name": obj.name, "joined": obj.joined.isoformat()}
    if isinstance(obj, datetime):
        return obj.isoformat()
    raise TypeError(f"Object of type {type(obj).__name__} is not JSON serializable")

json_str = json.dumps(u, default=encode_user, indent=4)
print(json_str)
