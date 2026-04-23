//Description: Prototypes for the 23 Tree class

#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


class TwoThreeTree{
    public:
		//constructor 
        TwoThreeTree();

		//destructor
		~TwoThreeTree();

        void contains() const;
		void contains(std::string input) const;
        bool isEmpty() const;
        void printTree(std::ostream & out = std::cout) const;
        void buildTree(std::ifstream & input);
		std::set<std::string> dumpTree() const;

    private:

	struct Key{
		std::string word;
		std::vector<int> lines;
	
		Key(){}

		Key(const std::string& w, int line) : word(w) {
			lines.push_back(line);
		}
	};

    struct node{
		Key first;
	    Key second;
		int keyCount; //1 or 2 allowed

	    node* left;
		node* middle;
	    node* right;

		//constructor for node with 1 key (all new nodes start with one key)
		node(const Key& k) : first(k), keyCount(1), left(nullptr), middle(nullptr), right(nullptr) {}
	};

	node* root;

	//helper structure to send data upwards
	struct InsertResult {
		bool split; // did a split occur?
		Key promotedKey; // the key you want to push up
		node* rightChild; // new right subtree from split

		InsertResult(bool s = false) : split(s), rightChild(nullptr) {}
	};

	void destroy(node* t);
	InsertResult insertHelper(const std::string& word, int line, node*& t, int& distWords);
	bool containsHelper(const std::string& word, node* t, node*& result) const;
	void printTreeHelper(node* t, std::ostream& out) const;
	int findHeight(node* t) const;
	bool isLeaf(node* t) const;
	void dumpTreeHelper(node* t, std::set<std::string>& out) const;
};
	
#endif	
    
	

