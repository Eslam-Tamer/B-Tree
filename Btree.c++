
#include <iostream>
using namespace std;

// 1. The base Node
template <typename Type, int Ord>
struct Node
{
    bool is_leaf;
    int num_keys; // Number of keys
    Type key[Ord]; // it's array for sorted keys

    Node(bool leaf)
    {
        is_leaf = leaf;
        num_keys = 0;
    }

    virtual ~Node() {}
};

// 2. Internal Node
template <typename Type, int Ord>
struct InternalNode : public Node<Type, Ord>
{
    Node<Type, Ord>* child[Ord + 1]; // internal node contain m+1 children (m-> order B-Tree)

    InternalNode() : Node<Type, Ord>(false)
    {
        for (int i = 0; i <= Ord; i++)
        {
            child[i] = nullptr;
        }
    }
};

// 3. Leaf Node
template <typename Type, int Ord>
struct LeafNode : public Node<Type, Ord>
{
    LeafNode() : Node<Type, Ord>(true) {}
};

// 4. B-Tree
template <typename Type, int Ord>
struct BTree
{
    Node<Type, Ord>* root;

    BTree()
    {
        root = nullptr;
    }

private:

    // Split a full Node
    Node<Type, Ord>* splitNode(
        Node<Type, Ord>* node,
        Type& promotedKey)
    {
        int middle = node->num_keys / 2;

        // Save the middle key to promote it
        promotedKey = node->key[middle]; // type not node

        Node<Type, Ord>* rightNode;

        if (node->is_leaf)
        {
            rightNode = new LeafNode<Type, Ord>();
        }
        else
        {
            rightNode = new InternalNode<Type, Ord>();
        }

        // Copy keys that belong to the right Node
        int rightCount = node->num_keys - middle - 1;

        for (int j = 0; j < rightCount; j++)
        {
            rightNode->key[j] = node->key[middle + 1 + j];
        }

        rightNode->num_keys = rightCount;

        // If these are Internal Nodes, move the children too
        if (!node->is_leaf)
        {
            InternalNode<Type, Ord>* leftInternal =
                static_cast<InternalNode<Type, Ord>*>(node);

            InternalNode<Type, Ord>* rightInternal =
                static_cast<InternalNode<Type, Ord>*>(rightNode);

            for (int j = 0; j <= rightCount; j++)
            {
                rightInternal->child[j] =
                    leftInternal->child[middle + 1 + j];
            }
        }

        // Keep only the left-side keys in the original Node
        node->num_keys = middle;

        return rightNode;
    }

    // Insert recursively
    Node<Type, Ord>* insertRecursive(
        Node<Type, Ord>* node,
        const Type& value,
        Type& promotedKey)
    {
        int i = node->num_keys - 1;

        if (node->is_leaf)
        {
            // Insert the value in sorted order
            while (i >= 0 && node->key[i] > value)
            {
                node->key[i + 1] = node->key[i];
                i--;
            }

            node->key[i + 1] = value;
            node->num_keys++;
        }
        else
        {
            // Find the correct child to insert in this child
            while (i >= 0 && node->key[i] > value)
            {
                i--;
            }

            i++;

            InternalNode<Type, Ord>* parent =
                static_cast<InternalNode<Type, Ord>*>(node); // reverse to internal node 

            Type childPromotedKey;

            // Go to the selected child
            Node<Type, Ord>* newRightChild =
                insertRecursive(parent->child[i],
                                value,
                                childPromotedKey);

            // If the child split, update this parent
            if (newRightChild != nullptr)
            {
                int j = parent->num_keys - 1;

                while (j >= i)
                {
                    parent->key[j + 1] = parent->key[j];
                    parent->child[j + 2] = parent->child[j + 1];
                    j--;
                }

                parent->key[i] = childPromotedKey;
                parent->child[i + 1] = newRightChild;
                parent->num_keys++;
            }
        }

        // Split this Node if it reached its key limit
        if (node->num_keys == Ord)
        {
            return splitNode(node, promotedKey);
        }

        return nullptr;
    }

    // Print all Nodes at a given depth
    // the main idea is , we start from root (0) and search for the target depth 
    void printAtDepth(
        Node<Type, Ord>* node,
        int currentDepth,
        int targetDepth)
    {
        if (node == nullptr)
            return;

        if (currentDepth == targetDepth)
        {
            cout << "[ ";

            for (int i = 0; i < node->num_keys; i++)
            {
                cout << node->key[i] << " ";
            }

            cout << "] ";
            return;
        }
        // we not in the target depth , go to the next level
        if (!node->is_leaf)
        {
            InternalNode<Type, Ord>* parent =
                static_cast<InternalNode<Type, Ord>*>(node);

            for (int i = 0; i <= parent->num_keys; i++)
            {
                printAtDepth(parent->child[i],
                             currentDepth + 1,
                             targetDepth);
            }
        }
    }

public:

    // Public insert function
    void insert(const Type& value)
    {
        // Case 1: The tree is empty
        if (root == nullptr)
        {
            root = new LeafNode<Type, Ord>();

            root->key[0] = value;
            root->num_keys = 1;

            return;
        }

        Type promotedKey;

        Node<Type, Ord>* newRightChild =
            insertRecursive(root, value, promotedKey);

        // If the root split, create a new root
        if (newRightChild != nullptr)
        {
            InternalNode<Type, Ord>* newRoot =
                new InternalNode<Type, Ord>();

            newRoot->key[0] = promotedKey;
            newRoot->child[0] = root;
            newRoot->child[1] = newRightChild;
            newRoot->num_keys = 1;

            root = newRoot;
        }
    }

    void displayAtDepth(int depth)
    {
        cout << "Depth " << depth << ": ";
        printAtDepth(root, 0, depth);
        cout << '\n';
    }
};

int main()
{
    BTree<int, 3> tree;

    int values[] = {10, 20, 5, 6, 12, 30, 7, 17, 3};

    for (int value : values)
    {
        tree.insert(value);
    }

    tree.displayAtDepth(0);
    tree.displayAtDepth(1);
    tree.displayAtDepth(2);

    return 0;
}
