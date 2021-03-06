/* O(n) Cartesian tree in C# */
using System;

class PST
{

/* internal Node class */
class Node
{
    //three attributes: value, left-child and right-child nodes
    public int data;
    public Node left, right;
};

/* Recursive test function for constructTree */
static void inorderTraversal (Node node)
{
    //base case
    if (node == null)
        return;

    //Inorder traversal of constructed tree
    inorderTraversal (node.left);
    Console.Write(node.data + " ");
    inorderTraversal (node.right);
}

// Recursively construct subtree using given root
static Node constructTreeUtil (int root, int []arr, int []parent, int []leftChild, int []rightChild)
{
    //return null when root is not defined
    if (root == -1)
        return null;

    // create a new node with root's data
    Node tmp = new Node();
    tmp.data = arr[root] ;

    // recursively construct left and right subtrees
    tmp.left = constructTreeUtil( leftChild[root], arr, parent, leftChild, rightChild );
    tmp.right = constructTreeUtil( rightChild[root], arr, parent, leftChild, rightChild );

    return tmp ;
}

// constuct the PST in O(n) time
static Node constructTree (int []arr, int n)
{
    // Arrays to hold the index of parent, left-child, right-child
    // Construct the tree in three sections
    int []parent = new int[n];
    int []leftChild = new int[n];
    int []rightChild = new int[n];

    // Initialize all array values as -1
    init(parent, -1);
    init(leftChild, -1);
    init(rightChild, -1);

    // 'root' and 'last' stores the index of the root and the
    // last processed node of the Cartesian Tree.
    int root = 0, last;

    // starting from the second element of the input array
    for (int i = 1; i <= n - 1; i++)
    {
        last = i - 1;
        rightChild[i] = -1;

        // find the minimum elements in arr
        while (arr[last] <= arr[i] && last != root)
            last = parent[last];

        // arr[i] is the largest element yet make it as the new root
        if (arr[last] <= arr[i])
        {
            parent[root] = i;
            leftChild[i] = root;
            root = i;
        }

        // reconfigure
        else if (rightChild[last] == -1)
        {
            rightChild[last] = i;
            parent[i] = last;
            leftChild[i] = -1;
        }

        // set last processed right-child as new left-child of the new root
        else
        {
            parent[rightChild[last]] = i;
            leftChild[i] = rightChild[last];
            rightChild[last] = i;
            parent[i] = last;
        }

    }

    // the root of the PST has no parent
    parent[root] = -1;

    // use the recursive method to construct the tree
    return (constructTreeUtil (root, arr, parent, leftChild, rightChild));
}

/* method to initialize arrays */
static void init(int[] arr, int value)
{
    for (int i = 0; i < arr.Length; i++)
    {
        arr[i] = value;
    }

}

/* Main Method */
public static void Main(String[] args)
{
    // nodes in the Cartesian tree
    int []arr = {8, 14, 34, 29, 21};
    int n = arr.Length;

    //construct the tree
    Node root = constructTree(arr, n);

    /*Inorder traversal */
    Console.Write("Inorder traversal of constructed PST : \n");
    inorderTraversal(root);
}
}
