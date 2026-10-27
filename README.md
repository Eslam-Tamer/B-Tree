# B-Tree Pseudocode

## 1. Insert

```text
Insert(value):

    If tree is empty:
        Create a new node
        Add value
        Set it as root
        Return

    Find the correct leaf

    If leaf has space:
        Insert value in sorted order
    Else:
        Split the node
        Move the middle key to the parent
        Create a right node

    If parent is full:
        Split the parent
        Move the middle key up

    If root splits:
        Create a new root
```

## 2. Display

```text
DisplayAtDepth(targetDepth):

    Start from root at depth 0

    PrintAtDepth(root, 0, targetDepth)


PrintAtDepth(node, currentDepth, targetDepth):

    If node is empty:
        Return

    If currentDepth == targetDepth:
        Print all keys in node
        Return

    For each child of node:
        PrintAtDepth(
            child,
            currentDepth + 1,
            targetDepth
        )
```
