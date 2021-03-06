#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;

/*PST: Priority Search Tree*/

// Data structure to store a PST node
struct Node {
	int data;
	Node *left, *right;

	// Initialization
	Node(int data)
	{
		this->data = data;
		this->left = nullptr;
		this->right = nullptr;
	}
};

// Recursive function to perform inorder traversal of a PST
void inorderTraversal(Node* root)
{
	if (root == nullptr)
		return;

	inorderTraversal(root->left);
	cout << root->data << endl;
	inorderTraversal(root->right);
}

// Function to find index of the minimum element in pst[start, end]
int minElementIndex(vector<int> const &pst, int start, int end)
{
	int minIdx = start;
	for (int i = start + 1; i <= end; i++)
	{
		if (pst[minIdx] > pst[i])
			minIdx = i;
	}

	return minIdx;
}

// Recursive function to construct a PST 

Node* constructTree(vector<int> const &pst, int start, int end)
{
	// base case
	if (start > end)
		return nullptr;

	// Find index of the minimum element in pst[start, end]
	int index = minElementIndex(pst, start, end);

	// Set the element with the minimum value as the root
	Node *root = new Node(pst[index]);

	// recursively construct the left subtree
	root->left = constructTree(pst, start, index - 1);

	// recursively construct the right subtree
	root->right = constructTree(pst, index + 1, end);

	// return current root node
	return root;
}

/*Main method*/
int main()
{
	// input sequence of number
	vector<int> pst = { 9, 4, 6, 1, 7, 13, 11, 22, 16, 19, 5 };

	// construct the tree
	Node *root = constructTree(pst, 0, pst.size() - 1);

	// Inorder traversal of the PST
	cout << "Inorder traversal of constructed Priority Search Tree: ";
	inorderTraversal(root);

	return 0;
}