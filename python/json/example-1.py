# Example 1 - Serialize (Encode) and Deserialize (Decode)

import json

data = {
    "name": "Alice",
    "age": 25,
    "is_student": False,
    "scores": [95, 88, 76],
    "address": None
}

# Serialize (Python -> JSON string)
json_str = json.dumps(data)
print(json_str)
# {"name": "Alice", "age": 25, "is_student": false, "scores": [95, 88, 76], "address": null}

# Deserialize (JSON string -> Python object)
restored = json.loads(json_str)
print(restored)
# {'name': 'Alice', 'age': 25, 'is_student': False, 'scores': [95, 88, 76], 'address': None}
