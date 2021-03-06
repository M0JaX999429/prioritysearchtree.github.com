// ConsoleApplication5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

/*Implementation of priority search trees*/

#include "pch.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

enum Color { RED, BLACK };

//Implement priority search tree using red-black tree

// data structure that represents a node in the tree
struct Node {
	int dataX, dataY;// x-coordinate and y-coordinate
	Node *parent; // pointer to the parent
	Node *left; // pointer to left child
	Node *right; // pointer to right child
	bool color;

	Node(int dataX, int dataY) {
		this->dataX = dataX;
		this->dataY = dataY;
		left = right = parent = NULL;
		this->color = RED;
	}
};

typedef Node *NodePtr;

// class RBTree implements the operations in Red Black Tree
// Class to represent Red-Black Tree 
class RBTree
{
private:
	Node *root;
protected:
	void rotateLeft(Node *&, Node *&);
	void rotateRight(Node *&, Node *&);
	void fixViolation(Node *&, Node *&);
public:
	// Constructor 
	RBTree() { root = NULL; }
	Node* sibling(Node* node);
	bool isOnLeft(Node* node);
	void insert(const int &x, const int &y);
	void delete_node(Node* node);
	void inorder();
	int treeSize();
	Node* getRoot();
};

// This function fixes violations  
// caused by BST insertion 
void RBTree::fixViolation(Node *&root, Node *&pt)
{
	Node *parent_pt = NULL;
	Node *grand_parent_pt = NULL;

	while ((pt != root) && (pt->color != BLACK) &&
		(pt->parent->color == RED))
	{

		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		/*  Case : A
			Parent of pt is left child
			of Grand-parent of pt */
		if (parent_pt == grand_parent_pt->left)
		{

			Node *uncle_pt = grand_parent_pt->right;

			/* Case : 1
			   The uncle of pt is also red
			   Only Recoloring required */
			if (uncle_pt != NULL && uncle_pt->color == RED)
			{
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				grand_parent_pt->color = RED;
				pt = grand_parent_pt;
			}

			else
			{
				/* Case : 2
				   pt is right child of its parent
				   Left-rotation required */
				if (pt == parent_pt->right)
				{
					pt = parent_pt;
					rotateLeft(root, pt);
					//parent_pt = pt->parent;
				}

				/* Case : 3
				   pt is left child of its parent
				   Right-rotation required */
				parent_pt->color = BLACK;
				grand_parent_pt->color = RED;
				rotateRight(root, grand_parent_pt);
				//pt = parent_pt;
			}
		}

		/* Case : B
		   Parent of pt is right child
		   of Grand-parent of pt */
		else
		{
			Node *uncle_pt = grand_parent_pt->left;

			/*  Case : 1
				The uncle of pt is also red
				Only Recoloring required */
			if ((uncle_pt != NULL) && (uncle_pt->color ==
				RED))
			{
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				grand_parent_pt->color = RED;
				pt = grand_parent_pt;
			}
			else
			{
				/* Case : 2
				   pt is left child of its parent
				   Right-rotation required */
				if (pt == parent_pt->left)
				{
					pt = parent_pt;
					rotateRight(root, pt);
					//parent_pt = pt->parent;;
				}

				/* Case : 3
				   pt is right child of its parent
				   Left-rotation required */
				parent_pt->color = BLACK;
				grand_parent_pt->color = RED;
				rotateLeft(root, grand_parent_pt);
				//pt = parent_pt;
			}
		}
	}
	root->color = BLACK;
}

/* A utility function to insert
	a new node with given key
   in BST */
Node* BSTInsert(Node* root, Node *pt)
{
	/* If the tree is empty, return a new node */
	if (root == NULL)
		return pt;

	/* Otherwise, recur down the tree */
	if (pt->dataY < root->dataY)
	{
		root->left = BSTInsert(root->left, pt);
		root->left->parent = root;
	}
	else if (pt->dataY > root->dataY)
	{
		root->right = BSTInsert(root->right, pt);
		root->right->parent = root;
	}


	/* return the (unchanged) node pointer */
	return root;
}

// Red-Black Tree left rotation
void RBTree::rotateLeft(Node *&root, Node *&pt)
{
	Node *pt_right = pt->right;

	pt->right = pt_right->left;

	if (pt_right->left != NULL) {
		pt_right->left->parent = pt;
	}

	pt_right->parent = pt->parent;

	if (pt->parent == NULL) {
		root = pt_right;
	}
	else if (pt == pt->parent->left) {
		pt->parent->left = pt_right;
	}
	else {
		pt->parent->right = pt_right;
	}

	pt_right->left = pt;
	pt->parent = pt_right;
}

// Red-Black Tree right rotation
void RBTree::rotateRight(Node *&root, Node *&pt)
{
	Node *pt_left = pt->left;

	pt->left = pt_left->right;

	if (pt_left->right != NULL) {
		pt_left->right->parent = pt;
	}

	pt_left->parent = pt->parent;

	if (pt->parent == NULL) {
		root = pt_left;
	}

	else if (pt == pt->parent->right) {
		pt->parent->right = pt_left;
	}

	else {
		pt->parent->left = pt_left;
	}

	pt_left->right = pt;
	pt->parent = pt_left;
}

//check whether node is the left child of parent
bool RBTree::isOnLeft(Node* node) { return node == node->parent->left; }


// function to insert a new node with given data 
void RBTree::insert(const int &dataX, const int &dataY)
{
	Node *pt = new Node(dataX, dataY);

	// Do a normal BST insert 
	root=BSTInsert(root, pt);

	// fix Red Black Tree violations 
	fixViolation(root, pt);
}

// function to swap value of two nodes in Red-Black Tree
void swapValues(Node *u, Node *v) {
	int tempX, tempY;

	tempX = u->dataX;
	tempY = u->dataY;

	u->dataX = v->dataX;
	u->dataY = v->dataY;

	v->dataX = tempX;
	v->dataY = tempY;
}

// returns pointer to sibling 
Node * RBTree::sibling(Node* node) {
	// sibling null if no parent 
	if (node->parent == NULL)
		return NULL;

	if (isOnLeft(node))
		return node->parent->right;

	return node->parent->left;
}

// find node that do not have a left child 
// in the subtree of the given node 
Node *successor(Node *x) {
	Node *temp = x;

	while (temp->left != NULL)
		temp = temp->left;

	return temp;
}

// find node that replaces a deleted node in BST 
Node *BSTreplace(Node *x) {
	// when node have 2 children 
	if (x->left != NULL and x->right != NULL)
		return successor(x->right);

	// when leaf 
	if (x->left == NULL and x->right == NULL)
		return NULL;

	// when single child 
	if (x->left != NULL)
		return x->left;
	else
		return x->right;
}

// Method to delete a specific node in the RED-BLACK Tree
void RBTree::delete_node(Node* node) {
	Node *temp = BSTreplace(node);

	// True when nodes are both black 
	bool bothBlack = ((temp == NULL or temp->color == BLACK) and (node->color == BLACK));
	Node *parent = node->parent;

	if (temp == NULL) {
		if (node == root) {
			// v is root, making root null 
			root = NULL;
		}
		else {
			if (bothBlack) {
				// u and v both black 
				// v is leaf, fix double black at v 
				fixViolation(root, node);
			}
			else {
				// u or v is red 
				if (sibling(node) != NULL)
					// sibling is not null, make it red
					sibling(node)->color = RED;
			}

			// delete v from the tree 
			if (isOnLeft(node)) {
				parent->left = NULL;
			}
			else {
				parent->right = NULL;
			}
		}
		delete node;
		return;
	}

	if (node->left == NULL or node->right == NULL) {
		// v has 1 child 
		if (node == root) {
			// v is root, assign the value of u to v, and delete u 
			node->dataX = temp->dataX;
			node->dataY = temp->dataY;
			node->left = node->right = NULL;
			delete temp;
		}
		else {
			// Detach v from tree and move u up 
			if (isOnLeft(node)) {
				parent->left = temp;
			}
			else {
				parent->right = temp;
			}
			delete node;
			temp->parent = parent;
			if (bothBlack) {
				// u and v both black, fix double black at u 
				fixViolation(root, temp);
			}
			else {
				// u or v red, color u black 
				temp->color = BLACK;
			}
		}
		return;
	}

	// v has 2 children, swap values with successor and recurse 
	swapValues(temp, node);
	delete_node(temp);

}

//method to calculate the constructed RB tree size
int tree_size(Node* node) {
	if (node == NULL) {
		return 0;
	}
	else {
		return (tree_size(node->left) + 1 + tree_size(node->right));
	}
}

// inorder traversal of the tree
void inorderTraversal(Node *root) {
	if (root == NULL) {
		return;
	}

	inorderTraversal(root->left);
	string color;
	if (root->color == 0) {
		color = "RED";
	}
	else {
		color = "BLACK";
	}

	cout << "(" << root->dataX << "," << root->dataY << ")";
	//printf_s("%s ", color.c_str());
	inorderTraversal(root->right);
}

// inner class methods
void RBTree::inorder() { inorderTraversal(root); };
int RBTree::treeSize() { return tree_size(root); };
Node* RBTree::getRoot() { return root; };

//Find the element with the lowest priority: the highest y value O(logn)
Node *minPriorityPoint(Node* root)
{
	Node* current_node = root;
	while (current_node->right != NULL) {
		current_node = current_node->right;
	}
	return current_node;
};

// Remove the point with minimum priority to construct reduced dataset S-P
RBTree remove_min_point(RBTree dataset, Node* min_point) {
	dataset.delete_node(min_point);
	return dataset;
}

// calculate the median point of X
int calculate_median(RBTree reduced_dataset) {

	Node* reduced_root = reduced_dataset.getRoot();
	stack <Node *> s;

	vector<int> x_value;

	if (reduced_root == NULL) {
		return 0;
	}

	while (reduced_root != NULL || s.empty() == false)
	{
		/* Reach the left most Node of the
		   curr Node */
		while (reduced_root != NULL)
		{
			/* place pointer to a tree node on
			   the stack before traversing
			  the node's left subtree */
			s.push(reduced_root);
			reduced_root = reduced_root->left;
		}

		///* Current must be NULL at this point */
		reduced_root = s.top();
		s.pop();

		//cout << "("<<reduced_root->dataX<<","<<reduced_root->dataY<<")";
		x_value.push_back(reduced_root->dataX);

		reduced_root = reduced_root->right;

	} /* end of while */

	sort(x_value.begin(), x_value.end());

	//vector<int>::iterator it;
	//for (it = x_value.begin(); it != x_value.end(); it++) {
	//	cout << *it << endl;
	//}

	if (reduced_dataset.treeSize() == 1) {
		return x_value.at(0);
	}
	return (x_value.at((reduced_dataset.treeSize() / 2) - 1) + x_value.at(reduced_dataset.treeSize() / 2)) / 2;

}

//Construct the Priority Search Tree
Node* construct_PST(RBTree dataset) {

	if (dataset.treeSize() > 1) {
		Node* min_point = minPriorityPoint(dataset.getRoot());//Find the point P such that P.y is the greatest
		cout << "[INFO] Point P with lowest priority:P(" << min_point->dataX << "," << min_point->dataY << ")\n";
		//cout << "[ERROR] Current PST Node:" << min_point->dataX << "," << min_point->dataY;

		//Node* PSTroot = min_point;
		//cout << "[ERROR] Current PST Node:" << PSTroot->dataX << "," << PSTroot->dataY;

		Node* PSTroot = new Node(0, 0);
		PSTroot->dataX = min_point->dataX;
		PSTroot->dataY = min_point->dataY;

		RBTree reduced_dataset = remove_min_point(dataset, min_point);//Remove point P from original set S, get reduced set (S-P)
		cout << "[INFO] Inorder traversal of the set S-P...\n";
		reduced_dataset.inorder();
		cout << "\n[COMPLETED] Inorder traversal of the set S-P done.\n";

		//cout << reduced_dataset.getRoot()->dataX;

		cout << "[INFO] Calculating X median of points in set S-P ...\n";
		int median = calculate_median(reduced_dataset);

		cout << "[COMPLETED] Median: " << median << "\n";

		//Divide points in S-P into half
		RBTree left_data;
		RBTree right_data;
		//vector <int> left_data1;
		//vector <int> right_data1;

		Node* reduced_root = reduced_dataset.getRoot();
		stack <Node* >s;

		if (reduced_dataset.treeSize() == 1) {
			if (reduced_root->dataX <= PSTroot->dataX) {
				left_data.insert(reduced_root->dataX, reduced_root->dataY);
			}
			else {
				right_data.insert(reduced_root->dataX, reduced_root->dataY);
			}
		}
		else {
			while (reduced_root != NULL || s.empty() == false)
			{
				/* Reach the left most Node of the
				   curr Node */
				while (reduced_root != NULL)
				{
					/* place pointer to a tree node on
					   the stack before traversing
					  the node's left subtree */
					s.push(reduced_root);
					reduced_root = reduced_root->left;
				}

				///* Current must be NULL at this point */
				reduced_root = s.top();
				s.pop();

				//cout << "("<<reduced_root->dataX<<","<<reduced_root->dataY<<")";
				if (reduced_root->dataX <= median) {
					left_data.insert(reduced_root->dataX, reduced_root->dataY);
					//left_data1.push_back(reduced_root->dataX);
				}
				else {
					right_data.insert(reduced_root->dataX, reduced_root->dataY);
					//right_data1.push_back(reduced_root->dataX);
				}


				//  Now, it's right
				//   subtree's turn */
				reduced_root = reduced_root->right;

			} /* end of while */
		}

		Node* left_sub_root = construct_PST(left_data);
		//cout << "Current PST Root ->left:" << left_sub_root->dataX;
		PSTroot->left = left_sub_root;

		Node* right_sub_root = construct_PST(right_data);
		//cout << "Current PST Root -> right:" << right_sub_root->dataX;
		PSTroot->right = right_sub_root;

		//cout << "[ERROR] Current PST Root:" << PSTroot->dataX << "," << PSTroot->dataY;

		return PSTroot;

		//vector<int>::iterator it1;
		//for (it1 = left_data1.begin(); it1 != left_data1.end(); it1++) {
		//	cout << *it1 << endl;
		//}

	}
	else if (dataset.treeSize() == 1) {
		//cout << "[ERROR] Current PST Node:" << dataset.getRoot()->dataX << "," << dataset.getRoot()->dataY;
		return dataset.getRoot();
	}
	else if (dataset.treeSize() == 0) {
		return NULL;
	}
}

//Query the Priority search tree
vector<Node*> PST_range_search(Node* PSTroot,int x_median,int min_key, int max_key,int min_priority)
{
	vector<Node*> result;

	if (PSTroot->left != NULL || PSTroot != NULL) {
		if (PSTroot->dataY < min_priority) {
			//cout << PSTroot->dataX << "," << PSTroot->dataY;
			//cout << "[REPORT] Root without range, no desired point.";
			return result;
		}

		if (min_key <= PSTroot->dataX && PSTroot->dataX <= max_key) {
			result.push_back(PSTroot);
			cout << "[REPORT] PST Node within range:(" << PSTroot->dataX << "," << PSTroot->dataY<<")\n";
		}

		if (min_key < x_median && PSTroot->left!=NULL) {
			//cout << "[INFO] Go to left sub tree:" << PSTroot->left->dataX << "," << PSTroot->left->dataY;
			vector<Node*> temp = PST_range_search(PSTroot->left,x_median, min_key, max_key, min_priority);
			result.insert(result.end(), temp.begin(), temp.end());
		}

		if (x_median < max_key && PSTroot->right!=NULL) {
			//cout << "[INFO] Go to right sub tree:" << PSTroot->right->dataX << "," << PSTroot->right->dataY;
			vector<Node*> temp = PST_range_search(PSTroot->right,x_median, min_key, max_key, min_priority);
			result.insert(result.end(), temp.begin(), temp.end());
		}

		return result;
	}
	else
	{
		//cout << "[ERROR] PST leaf Node:" << PSTroot->dataX << "," << PSTroot->dataY;
		if (PSTroot->dataY < min_priority) {
			if (min_key <= PSTroot->dataX&&PSTroot->dataX < max_key) {
				result.push_back(PSTroot);
			}
		}
	}
}

int main()
{
	cout << "[INFO]Testing dataset S={A(25,35), B(50,10), C(85,15), D(90,5),E(5,45), F(35,40), G(60,75),H(80,65)}\n";
	/*Dataset S={A(25,35), B(50,10), C(85,15), D(90,5),E(5,45), F(35,40), G(60,75),H(80,65)}*/
	cout << "[INFO] Constructing testing dataset as RB tree...\n";

	//Store the data points in RB tree
	RBTree dataset;
	dataset.insert(25, 35);
	dataset.insert(50, 10);
	dataset.insert(85, 15);
	dataset.insert(90, 5);
	dataset.insert(5, 45);
	dataset.insert(35, 40);
	dataset.insert(60, 75);
	dataset.insert(80, 65);
	cout << "[COMPLETED] The RED-BLACK tree done.\n";

	cout << "[INFO] Inorder Traversal of the RED-BLACK tree: \n";
	dataset.inorder();
	cout << "\n[COMPLETED] Inorder Traversal of the RED-BLACK tree done.\n";

	//construct of the tree
	cout << "[INFO] Constructing Priority Search Tree of given dataset...\n ";
	Node* PSTroot = construct_PST(dataset);
	cout << "[COMPLETED] The Priority Search Tree constructed.\n";

	cout << "[INFO] Inorder Traversal of Priority Search Tree...\n";

	inorderTraversal(PSTroot);

	//insertion and deletion?

	//Grounded range search
	cout << "\n[INFO] Query the Priority Search Tree (solving range search problem)...\n";
	cout << "[INFO] Please enter the query range [min_key, max_key] and [min_priority,+∞]:\n";
	int min_key, max_key, min_priority = 0;
	int pst_treeSize = dataset.treeSize();
	cout << "min_key = ";
	cin >> min_key;
	cout << "max_key = ";
	cin >> max_key;
	cout << "min_priority = ";
	cin >> min_priority;

	cout << "[INFO] Range querying points in range [" << min_key << "," << max_key << "] and [" << min_priority << ",+∞]...\n";

	cout << "[INFO] Calculating X(R)...\n";

	Node* min_point = minPriorityPoint(dataset.getRoot());;

	RBTree reduced_dataset = remove_min_point(dataset, min_point);//Remove point P from original set S, get reduced set (S-P)
	int median = calculate_median(reduced_dataset);

	cout << "[INFO] X(R)=" << median<<"\n";

	int x_median = median;

	vector <Node*> ans = PST_range_search(PSTroot,x_median,min_key,max_key, min_priority);



	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
