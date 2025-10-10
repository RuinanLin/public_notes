# Example 7 - Handling Floating-Point and Special Values

# JSON strictly forbids `NaN`, `Infinity`, and `-Infinity`.

# Python's json allows them by default (for backward compatibility),
# but you can disallow them:

import json
import math

data = {"a": math.nan, "b": math.inf}

# Default: allowed (non-standard JSON)
print(json.dumps(data)) # {"a": NaN, "b": Infinity}

# Strict JSON
json.dumps(data, allow_nan=False)
# ValueError: Out of range float values are not JSON compliant
