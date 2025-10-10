# Example 2 - Using .dump() and .load()

import json

data = {"id": 1, "title": "JSON Tutorial", "tags": ["python", "data"]}

# Write JSON to file
with open("data.json", "w") as f:
    json.dump(data, f, indent=4)

# Read JSON from file
with open("data.json", "r") as f:
    obj = json.load(f)

print(obj)
