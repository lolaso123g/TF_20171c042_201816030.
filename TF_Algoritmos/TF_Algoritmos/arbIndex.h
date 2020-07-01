#pragma once
#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include "Fila.h"

using namespace std;
#define max(a, b) (a > b? a : b)

template<typename T, typename R = T, T NONE = 0>
class AVLBST {
	struct Node {
		T       element;
		Node*   left;
		Node*   right;
		int     height;

		Node(T element)
			: element(element), height(0),
			left(nullptr), right(nullptr) {}
	};
	Node*           root;
	function<R(T, int)>  key;
	function<string(T, int)>  ToString;
	int             len;
	int				numColumna;
public:
	AVLBST(function<R(T, int)> key, function<string(T, int)>  ToString)
		: key(key), root(nullptr), len(0), ToString(ToString) {}
	~AVLBST() { clear(root); }
	int height() { return height(root); }
	int size() { return len; }
	void setNColumna(int numC) { numColumna = numC; }
	int getNColumna() { return numColumna; }
	void clear() { clear(root); len = 0; }

	void add(T elem) { add(root, elem); }
	T find(R val) { return find(root, val); }
	void greaterThan(vector<T>& filtro, R val) {
		filtro.clear(); 
		greaterThan(filtro, root, val); 
	}
	void lowerThan(vector<T>& filtro, R val) {
		filtro.clear(); 
		lowerThan(filtro, root, val); 
	}
	void equalTo(vector<T>& filtro, R val) {
		filtro.clear();
		equalTo(filtro, root, val);
	}
	void startsWith(vector<T>& filtro, string val) {
		filtro.clear();
		startsWith(filtro, root, val);
	}
	void endsIn(vector<T>& filtro, string val) {
		filtro.clear();
		endsIn(filtro, root, val);
	}
	void includedIn(vector<T>& filtro, R val) {
		filtro.clear();
		includedIn(filtro, root, val);
	}
	void notIncludedIn(vector<T>& filtro, R val) {
		filtro.clear();
		notIncludedIn(filtro, root, val);
	}
	//void inOrder(function<void(T)> proc) { inOrder(root, proc); }
	void inOrder(vector<T>& enOrden) { inOrder(enOrden, root); }

private:

	void clear(Node*& node) {
		if (node != nullptr) {
			clear(node->left);
			clear(node->right);
			delete node;
			node = nullptr;
		}
	}

	int height(Node* node) {
		return node == nullptr ? -1 : node->height;
	}

	void add(Node*& node, T elem) {
		if (node == nullptr) {
			node = new Node(elem);
			++len;
		}
		else {
			if (key(elem, numColumna) < key(node->element, numColumna)) {
				add(node->left, elem);
			}
			else {
				add(node->right, elem);
			}
			balance(node);
		}
	}

	T find(Node* node, R val) {
		if (node == nullptr) {
			return NONE;
		}
		else if (val == key(node->element, numColumna)) {
			return node->element;
		}
		else if (val < key(node->element, numColumna)) {
			return find(node->left, val);
		}
		else {
			return find(node->right, val);
		}
	}
	void greaterThan(vector<T>& filtro, Node* node, R val) {
		if (node != nullptr) {
			if (val < key(node->element, numColumna)) {
				filtro.push_back(node->element);
				greaterThan(filtro, node->right, val);
				greaterThan(filtro, node->left, val);
			}
			else {
				greaterThan(filtro, node->right, val);
			}
		}
	}
	void lowerThan(vector<T>& filtro, Node* node, R val) {
		if (node != nullptr) {
			if (val > key(node->element, numColumna)) {
				filtro.push_back(node->element);
				lowerThan(filtro, node->right, val);
				lowerThan(filtro, node->left, val);
			}
			else {
				lowerThan(filtro, node->left, val);
			}
		}
	}
	void equalTo(vector<T>& filtro, Node* node, R val) {
		if (node != nullptr) {
			if (val == key(node->element, numColumna)) {
				filtro.push_back(node->element);
				equalTo(filtro, node->right, val);
				equalTo(filtro, node->left, val);
			}
			else if (val > key(node->element, numColumna)) {
				equalTo(filtro, node->right, val);
			}
			else if (val < key(node->element, numColumna)) {
				equalTo(filtro, node->left, val);
			}
		}
	}
	void startsWith(vector<T>& filtro, Node* node, string val) {
		if (node != nullptr) {
			if (val[0] == ToString(node->element, numColumna)[0]) {
				filtro.push_back(node->element);
				startsWith(filtro, node->right, val);
				startsWith(filtro, node->left, val);
			}
			else {
				startsWith(filtro, node->left, val);
				startsWith(filtro, node->right, val);
			}
		}
	}
	void endsIn(vector<T>& filtro, Node* node, string val) {
		if (node != nullptr) {
			if (val.back() == ToString(node->element, numColumna).back()) {
				filtro.push_back(node->element);
				endsIn(filtro, node->right, val);
				endsIn(filtro, node->left, val);
			}
			else {
				endsIn(filtro, node->left, val);
				endsIn(filtro, node->right, val);
			}
		}
	}
	void includedIn(vector<T>& filtro, Node* node, R val) {
		if (node != nullptr) {
			if (key(node->element, numColumna).find(val) != -1) {
				filtro.push_back(node->element);
				includedIn(filtro, node->right, val);
				includedIn(filtro, node->left, val);
			}
			else {
				includedIn(filtro, node->right, val);
				includedIn(filtro, node->left, val);
			}
		}
	}
	void notIncludedIn(vector<T>& filtro, Node* node, R val) {
		if (node != nullptr) {
			if (key(node->element, numColumna).find(val) == -1) {
				filtro.push_back(node->element);
				notIncludedIn(filtro, node->right, val);
				notIncludedIn(filtro, node->left, val);
			}
			else  {
				notIncludedIn(filtro, node->right, val);
				notIncludedIn(filtro, node->left, val);
			}
		}
	}
	void inOrder(vector<T>& enOrden, Node* node) {
		if (node != nullptr) {
			inOrder(enOrden, node->left);
			enOrden.push_back(node->element);
			inOrder(enOrden, node->right);
		}
	}
	/*void inOrder(Node* node, function<void(T)> proc) {
		if (node != nullptr) {
			inOrder(node->left, proc);
			proc(node->element);
			inOrder(node->right, proc);
		}
	}*/

	void updateHeight(Node* node) {
		if (node != nullptr) {
			int hl = height(node->left);
			int hr = height(node->right);

			node->height = max(hl, hr) + 1;
		}
	}

	void rotateLeft(Node*& node) {
		Node* aux = node->right;
		node->right = aux->left;
		updateHeight(node);
		aux->left = node;
		updateHeight(aux);
		node = aux;
	}

	void rotateRight(Node*& node) {
		Node* aux = node->left;
		node->left = aux->right;
		updateHeight(node);
		aux->right = node;
		updateHeight(aux);
		node = aux;
	}

	void balance(Node*& node) {
		int hl = height(node->left);
		int hr = height(node->right);

		if (hr - hl < -1) {
			hl = height(node->left->left);
			hr = height(node->left->right);
			if (hr > hl) {
				rotateLeft(node->left);
			}
			rotateRight(node);
		}
		else if (hr - hl > 1) {
			hl = height(node->right->left);
			hr = height(node->right->right);
			if (hl > hr) {
				rotateRight(node->right);
			}
			rotateLeft(node);
		}
		else {
			updateHeight(node);
		}
	}
};
