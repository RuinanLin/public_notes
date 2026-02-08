# Better Code - Python - 2
# Write Google Style Docstring for Python Functions

## Author: Ruinan Lin
## Date: Feb 8, 2026

### Problem Description

It's pretty easy to forget the meaning of a function after several weeks. Although you can read it again, it still wastes some time. So it is a good practice to write clear docstring (documentation string) for your Python functions. It is like leaving a trail of breadcrumbs for your future self or your coworkers.

While you can write a simple sentence, the industry standard is to follow a structured format like **Google Style** or **NumPy Style**. Here, we introduce [Google Style](https://google.github.io/styleguide/pyguide.html).

### The Anatomy of a Great Docstring

A professional comment paragraph usually consists of three parts:

1. **Summary**: A concise one-line description of what the function does;
2. **Args (Arguments)**: A list of parameters, their types, and what the represent;
3. **Returns**: The type and description of the value the function sends back.

An example:

```python
def calculate_velocity(distance, time):
    """
    Calculates the average velocity of an object.
    
    Args:
        distance (float): The total distance traveled in meters.
        time (float): The total time taken in seconds.
    
    Returns:
        float: The calculated velocity in meters per second (m/s).
    
    Raises:
        ZeroDivisionError: If time is provided as 0.
    """
    return distance / time
```

Another example come from my Azul frontend code:

```python
def contiguous_partition(elements, k):
    """
    Splits a list into k contiguous partitions as evenly as possible.

    If the number of elements is not perfectly divisible by k, the first
    'n % k' partitions will each have one extra element. If k is greater
    than the number of elements, the remaining partitions will be empty.

    Args:
        elements (list or sequence): The collection of items to partition.
        k (int): The desired number of partitions.

    Returns:
        list[list]: A list containing k sub-lists.

    Raises:
        ValueError: If k is less than 1.
    """
    n = len(elements)
    if k < 1:
        raise ValueError("k must be greater than 0")

    # If k > n, we'll have some empty buckets at the end
    num_not_empty_buckets = min(k, n)

    # Calculate the minimum size of each bucket
    base_size = n // num_not_emtpy_buckets
    # Calculate how many buckets need one extra element
    extras = n % num_not_emtpy_buckets

    buckets = []
    current_idx = 0

    for i in range(num_not_emtpy_buckets):
        # Determine the size of current bucket
        bucket_size = base_size + (1 if i < extras else 0)
        # Add elements to the current bucket
        bucket = elements[current_idx:current_idx + bucket_size]
        buckets.append(bucket)
        current_idx += bucket_size

    # Add empty buckets if k was originally larger than n
    while len(buckets) < k:
        buckets.append([])

    return buckets
```

### Add More Clarity: Using Type Hints

Python type hints are like adding a set of instructions to your code. They don't change how the code runs — Python is still dynamically typed — but they make your life much easier by providing better autocompletion, catching bugs early with linters, and making your logic clearer to others and your future self.

To add a type hint, use a colon `:` after the parameter name and an arrow `->` before the colon of the function definition to indicate the return type.

```python
def greet(name: str, age: int) -> str:
    return f"Hello {name}, you are {age} years old."
```

For versions of Python 3.9 and later, you can use built-in types like `list`, `dict`, and `tuple` directly to specify what is *inside* the collection.

- **Lists**: `list[str]` (a list of strings);
- **Dictionaries**: `dict[str, int]` (a dictionary with string keys and integer values);
- **Tuples**: `tuple[int, int, int]` (a tuple with three specific integers).

Sometimes a variable could be more than one thing. Since Python 3.10, you can use the pipe operator `|` for this.

```python
def calculate_area(radius: int | float) -> float:
    return 3.14 * (radius ** 2)
```

If a parameter can be a specific type or `None`, you can use `| None`. This is very common for default arguments.

```python
def find_user(user_id: int, flag: str | None = None) -> str:
    ...
```

If you've written type hints, you probably don't need to write the types of arguments and returns again in docstrings. In modern Python, repeating type information in docstrings is generally considered redundant and can lead to "documentation rot — where you update the code but forget to update the docstring, creating conflicting information.

Our example above should be improved like this:

```python
def contiguous_partition(elements: list[object], k: int) -> list[list[object]]:
    """
    Splits a list into k contiguous partitions as evenly as possible.

    If the number of elements is not perfectly divisible by k, the first
    'n % k' partitions will each have one extra element. If k is greater
    than the number of elements, the remaining partitions will be empty.

    Args:
        elements: The collection of items to partition.
        k: The desired number of partitions.

    Returns:
        A list containing k sub-lists.

    Raises:
        ValueError: If k is less than 1.
    """
    n = len(elements)
    if k < 1:
        raise ValueError("k must be greater than 0")

    # If k > n, we'll have some empty buckets at the end
    num_not_empty_buckets = min(k, n)

    # Calculate the minimum size of each bucket
    base_size = n // num_not_emtpy_buckets
    # Calculate how many buckets need one extra element
    extras = n % num_not_emtpy_buckets

    buckets = []
    current_idx = 0

    for i in range(num_not_emtpy_buckets):
        # Determine the size of current bucket
        bucket_size = base_size + (1 if i < extras else 0)
        # Add elements to the current bucket
        bucket = elements[current_idx:current_idx + bucket_size]
        buckets.append(bucket)
        current_idx += bucket_size

    # Add empty buckets if k was originally larger than n
    while len(buckets) < k:
        buckets.append([])

    return buckets
```

### Keep it Updated

Please note that a wrong comment is often worse than no comment at all!

