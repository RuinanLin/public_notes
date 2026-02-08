# Better Code - Python - 1
# Use Dictionary Mapping to Substitute `if-else-if`

## Author: Ruinan Lin
## Date: Feb 8, 2026

### Question Description

In my code, I write something like this:

```python
def partition_pdhg(K, nparts, partitioning):
    if partitioning == "hgr":
        return partition_pdhg_hgr(K, nparts)
    elif partitioning == "block":
        return partition_pdhg_block(K, nparts)
    elif partitioning == "one-by-one":
        return partition_pdhg_one_by_one(K, nparts)
    else:
        raise ValueError(f"Unknown partitioning: {partitioning}")
```

It does not hurt performance that much, but it is already really ugly.

### Better Code

We can use **Dictionary Mapping**, which partitions logic and data and improves readability:

```python
def partition_pdhg(K, nparts, partitioning):
    strategies = {
        "hgr": partition_pdhg_hgr,
        "block": partition_pdhg_block,
        "one-by-one": partition_pdhg_one_by_one,
    }

    handler = strategies.get(partitioning)
    if not handler:
        raise ValueError(f"Unknown partitioning: {partitioning}")

    return func(K, nparts)
```
