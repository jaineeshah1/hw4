#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


int height(Node* root) {
	if (root == nullptr) {
		return 1;
	}
	if (root->left != nullptr) {
		return 1 + height(root->left);
	}
	else {
		return 1 + height(root->right);
	}
	
}

bool equalPaths(Node* root, int height, int &counter) {
	if (root == nullptr) {
		return true;
	}
	if (root->left == nullptr && root->right == nullptr) {
		counter += 1;
		return counter == height;
	}

	if (root->left != nullptr && root->right != nullptr) {
		counter += 1;
		int leftCounter = counter;
		int rightCounter = counter;
		bool a = equalPaths(root->left, height, leftCounter);
		bool b = equalPaths(root->right, height, rightCounter);
		return a && b;
	}
	if (root->left != nullptr) {// right is nullptr 
		counter += 1;
		return equalPaths(root->left, height, counter);
	}
	else {
		counter += 1;
		return equalPaths(root->right, height, counter);
	}
}

bool equalPaths(Node* root) {
	if (root == nullptr) {
		return true;
	}
	if (root->left == nullptr && root->right == nullptr) {
		return true;
	}
	// get one path's length
	int _height = height(root);
	int counter = 1;
	return equalPaths(root, _height, counter);
}