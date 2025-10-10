# Example 3 - Pretty Printing and Sorting

import json

data = {"z": 1, "a": 2, "é": "accent"}

json_str = json.dumps(data, indent=4, sort_keys=True, separators=('%', '&'), ensure_ascii=False)
print(json_str)
json_str = json.dumps(data, indent=4, sort_keys=True, ensure_ascii=False)
print(json_str)
json_str = json.dumps(data, sort_keys=True, ensure_ascii=False)
print(json_str)
json_str = json.dumps(data, ensure_ascii=False)
print(json_str)
json_str = json.dumps(data)
print(json_str)
