# Example 6 - Handling Unsupported Types

import json

data = {
    "nums": (1, 2, 3),
    "unique": {4, 5, 6},
    "binary": b"hello"
}

def encode_any(obj):
    if isinstance(obj, (set, tuple)):
        return list(obj)
    if isinstance(obj, bytes):
        return obj.decode("utf-8")
    raise TypeError(f"{obj!r} is not JSON serializable")

json_str = json.dumps(data, default=encode_any)
print(json_str)
# {"nums": [1, 2, 3], "unique": [4, 5, 6], "binary": "hello"}
