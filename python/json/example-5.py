# Example 5 - Restoring a Custom Class

# When you load JSON, you can use `object_hook` to customize
# how dictionaries are turned back into object.

import json
from datetime import datetime

class User:
    def __init__(self, name, joined):
        self.name = name
        self.joined = joined

    def __repr__(self):
        return f"User({self.name!r}, {self.joined!r})"

json_str = '{"name": "Alice", "joined": "2024-01-15T00:00:00"}'

def decode_user(d):
    if "joined" in d:
        d["joined"] = datetime.fromisoformat(d["joined"])
        return User(**d)
    return d

user = json.loads(json_str, object_hook=decode_user)
print(user)
# User('Alice', datetime.datetime(2024, 1, 15, 0, 0))
