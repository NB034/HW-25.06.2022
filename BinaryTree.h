#pragma once
#include <iostream>

using namespace std;

template<class T1, class T2>
struct node {
	T1 key{};
	T2 val{};
	node<T1, T2>* parent = nullptr;
	node<T1, T2>* left = nullptr;
	node<T1, T2>* right = nullptr;
};

template<class T1, class T2>
class BinaryTree {
	node<T1, T2>* root = nullptr;
	int size = 0;

public:
	BinaryTree() = default;
	BinaryTree(const BinaryTree<T1, T2>& obj);
	BinaryTree(BinaryTree<T1, T2>&& obj);
	~BinaryTree();

	BinaryTree<T1, T2>& operator=(const BinaryTree<T1, T2>& obj);
	BinaryTree<T1, T2>& operator=(BinaryTree<T1, T2>&& obj);
	T2& operator[](const T1& key);
	T2& operator[](const T1& key) const;

	void clear();
	bool empty() const;
	void print() const;
	void printNode(node<T1, T2>* node) const;
	int count(const T1& key);
	void insert(const T1& key, const T2& val);
	void deleteNode(const T1& key);
	void deleteNode(node<T1, T2>* obj, const T1& key);

	node<T1, T2>* min(node<T1, T2>* obj);
	node<T1, T2>* max(node<T1, T2>* obj);
	node<T1, T2>* next(node<T1, T2>* obj);
	node<T1, T2>* prev(node<T1, T2>* obj);
	node<T1, T2>* const upperBound(const T1& key);
	node<T1, T2>* const lowerBound(const T1& key);
	node<T1, T2>* find(const T1& key);

private:
	void copyNode(const node<T1, T2>* obj);
	void destructNode(node<T1, T2>* obj);
};



template<class T1, class T2>
BinaryTree<T1, T2>::BinaryTree(const BinaryTree<T1, T2>& obj) { this->copyNode(obj.root); }

template<class T1, class T2>
BinaryTree<T1, T2>::BinaryTree(BinaryTree<T1, T2>&& obj) {
	swap(this->root, obj.root);
	swap(this->size, obj.size);
}

template<class T1, class T2>
BinaryTree<T1, T2>::~BinaryTree() {
	this->destructNode(this->root);
	this->root = nullptr;
	this->size = 0;
}

template<class T1, class T2>
BinaryTree<T1, T2>& BinaryTree<T1, T2>::operator=(const BinaryTree<T1, T2>& obj) {
	if (this == &obj)
		return *this;
	this->~BinaryTree();
	this->copyNode(obj.root);
	return *this;
}

template<class T1, class T2>
BinaryTree<T1, T2>& BinaryTree<T1, T2>::operator=(BinaryTree<T1, T2>&& obj) {
	if (this->root != nullptr)
		this->~BinaryTree();
	swap(this->root, obj.root);
	swap(this->size, obj.size);
	return *this;
}

template<class T1, class T2>
T2& BinaryTree<T1, T2>::operator[](const T1& key) {
	return this->find(key);
}

template<class T1, class T2>
T2& BinaryTree<T1, T2>::operator[](const T1& key) const {
	return this->find(key);
}

template<class T1, class T2>
void BinaryTree<T1, T2>::clear() {
	this->~BinaryTree();
}

template<class T1, class T2>
bool BinaryTree<T1, T2>::empty() const {
	return this->root == nullptr;
}

template<class T1, class T2>
void BinaryTree<T1, T2>::print() const {
	this->printNode(this->root);
}

template<class T1, class T2>
void BinaryTree<T1, T2>::printNode(node<T1, T2>* node) const {
	if (node != nullptr) {
		this->printNode(node->left);
		cout << node->key << " : " << node->val << endl;
		this->printNode(node->right);
	}
}

template<class T1, class T2>
int BinaryTree<T1, T2>::count(const T1& key) {
	node<T1, T2>* y = this->find(key);
	int count = 0;
	while (y != nullptr && y->key == key) {
		y = this->next(y);
		count++;
	}
	return count;
}

template<class T1, class T2>
void BinaryTree<T1, T2>::insert(const T1& key, const T2& val) {
	node<T1, T2>* newNode = new node<T1, T2>;
	newNode->key = key;
	newNode->val = val;
	this->size++;
	if (this->root == nullptr) {
		this->root = newNode;
		return;
	}
	node<T1, T2>* parent = nullptr;
	node<T1, T2>* scout = this->root;
	while (scout != nullptr) {
		parent = scout;
		if (key < scout->key)
			scout = scout->left;
		else
			scout = scout->right;
	}
	if (newNode->key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;
	newNode->parent = parent;
}

template<class T1, class T2>
void BinaryTree<T1, T2>::deleteNode(const T1& key) {
	node<T1, T2>* node = this->find(key);
	this->deleteNode(node, node->key);
}

template<class T1, class T2>
void BinaryTree<T1, T2>::deleteNode(node<T1, T2>* obj, const T1& key) {
	if (obj == nullptr)
		return;
	if (obj->left == nullptr && obj->right == nullptr) {
		if (obj != this->root) {
			if (obj->key < obj->parent->key)
				obj->parent->left = nullptr;
			else
				obj->parent->right = nullptr;
		}
		else
			this->root = nullptr;
		delete obj;
		return;
	}
	node<T1, T2>* suitableNode = this->min(obj->right);
	if (suitableNode == obj->right)
		suitableNode = nullptr;
	if (suitableNode == nullptr) {
		if (obj != this->root) {
			if (obj->parent != nullptr) {
				obj->parent->right = obj->right;
				obj->right->parent = obj->parent;
			}
		}
		else
			this->root = obj->right;
		delete obj;
		return;
	}
	obj->key = suitableNode->key;
	obj->val = suitableNode->val;
	this->deleteNode(suitableNode, suitableNode->key);
}

template<class T1, class T2>
node<T1, T2>* BinaryTree<T1, T2>::min(node<T1, T2>* obj) {
	while (obj->left != nullptr)
		obj = obj->left;
	return obj;
}

template<class T1, class T2>
node<T1, T2>* BinaryTree<T1, T2>::max(node<T1, T2>* obj) {
	while (obj->right != nullptr)
		obj = obj->right;
	return obj;
}

template<class T1, class T2>
node<T1, T2>* BinaryTree<T1, T2>::next(node<T1, T2>* obj) {
	if (obj->right != nullptr)
		return this->min(obj->right);
	while (obj->parent != nullptr && obj != obj->parent->left)
		obj = obj->parent;
	return obj->parent;
}

template<class T1, class T2>
node<T1, T2>* BinaryTree<T1, T2>::prev(node<T1, T2>* obj) {
	if (obj->left != nullptr)
		return this->max(obj->left);
	while (obj->parent != nullptr && obj != obj->parent->right)
		obj = obj->parent;
	return obj->parent;
}

template<class T1, class T2>
node<T1, T2>* const BinaryTree<T1, T2>::upperBound(const T1& key) {
	node<T1, T2>* y = this->find(key);
	while (y != nullptr && y->key == key)
		y = this->next(y);
	return y;
}

template<class T1, class T2>
node<T1, T2>* const BinaryTree<T1, T2>::lowerBound(const T1& key) {
	return this->prev(this->find(key));
}

template<class T1, class T2>
node<T1, T2>* BinaryTree<T1, T2>::find(const T1& key) {
	node<T1, T2>* searcher = this->root;
	while (searcher->key != key && searcher != nullptr) {
		if (key < searcher->key)
			searcher = searcher->left;
		else
			searcher = searcher->right;
	}
	if (searcher == this->root && this->root->key != key)
		return nullptr;
	return searcher;
}

template<class T1, class T2>
void BinaryTree<T1, T2>::copyNode(const node<T1, T2>* obj) {
	if (obj != nullptr) {
		this->insert(obj->key, obj->val);
		this->copyNode(obj->left);
		this->copyNode(obj->right);
	}
}

template<class T1, class T2>
void BinaryTree<T1, T2>::destructNode(node<T1, T2>* obj) {
	if (obj != nullptr) {
		this->destructNode(obj->left);
		this->destructNode(obj->right);
		delete obj;
	}
}