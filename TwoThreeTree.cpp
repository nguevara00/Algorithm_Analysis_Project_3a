//Description: Implementation of the 2-3 Tree class

#include "TwoThreeTree.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cctype>

//Constructor
TwoThreeTree::TwoThreeTree(){
    root = nullptr;
}

//Used to implement the search function in main
void TwoThreeTree::contains() const{
   std::string input;
   node* foundNode = nullptr;
   std::cout << "Search word: ";
   std::cin >> input;

   if(containsHelper(input, root, foundNode)){
		const Key* foundKey = nullptr;

		if (foundNode->first.word == input) {
			foundKey = &(foundNode->first);
		}
		else {
			foundKey = &(foundNode->second);
		}

	    std::cout << "Line Numbers: " << foundKey->lines[0];
	    
		for(int i = 1; i < static_cast<int>(foundKey->lines.size()); i++){
			std::cout << ", " << foundKey->lines[i]; 
		}

	    std::cout << '\n';
	    std::cout << "Frequency: " << foundKey->lines.size() << '\n';
	}
	else {
	    std::cout << '"' << input << "\" is not in the document\n";
	}
}

//Returns true if there are no nodes in the tree
bool TwoThreeTree::isEmpty() const {
    return root == nullptr;
}

//Prints the index to the supplied receiver, either
//cout or the output file
void TwoThreeTree::printTree(std::ostream & out) const {
	out << "2-3 Tree Index:\n-------------------------\n";
	printTreeHelper(root, out);
}


//Receives the specified input file and constructs 
//the actual tree. Prints a message when finished.
void TwoThreeTree::buildTree(std::ifstream & input){
	int line = 1; 
	int numWords = 0;
	int distWords = 0; 
	int treeHeight = 0;

	double startTime = clock();
	
	std::string tempLine;
	while (getline(input, tempLine)) {
		std::string tempWord;

		for (int i = 0; i < static_cast<int>(tempLine.length()); i++) {
		    //Insert valid chars into tempWord until a delimiter( newline or space) is found
		    while (i < static_cast<int>(tempLine.length()) && tempLine[i] != ' ' && tempLine[i] != '\n' ) {
				tempWord.insert(tempWord.end(), tempLine[i]);
				i++;
		    }
		   
            //Trim any punctuation off end of word. Will leave things like apostrophes
            //and decimal points
            while(tempWord.length() > 0 && !isalnum(static_cast<unsigned char>(tempWord[tempWord.length() - 1]))) {
			    tempWord.resize(tempWord.size() -1);
			}   
			
            if (tempWord.length() > 0) {
				node* foundNode = nullptr;
				bool existed = containsHelper(tempWord, root, foundNode);

				InsertResult result = insertHelper(tempWord, line, root);

				if (!existed){
					distWords++;
				}

				if (result.split){
					node* newRoot = new node(result.promotedKey);
					newRoot->left = root;
					newRoot->right = result.rightChild;
					root = newRoot;
				}

                //Increment our total number of words inserted
                numWords++;
                //Clear out tempWord so we can use it again
                tempWord.clear();
		    }
		}
		line++;
	}
	//Do time and height calculation
	double finishTime = clock();
	double totalTime = (finishTime - startTime) / CLOCKS_PER_SEC;
	treeHeight = findHeight(root);

	//Print output
	std::cout << std::setw(40) << std::left;
	std::cout << "Total number of words: " << numWords << std::endl;

	std::cout << std::setw(40) << std::left; 
	std::cout << "Total number of distinct words: " << distWords << std::endl;

	std::cout << std::setw(40) << std::left; 
	std::cout <<"Total time spent building index: " << totalTime << std::endl;

	std::cout << std::setw(40) << std::left;
	std::cout <<"Height of 2-3 Tree is : " << treeHeight << std::endl;
}

//x is the word to insert, line is the line in the text file
//the word was found at, node is the node of the tree being
//examined, and distWord is incremented if a new word is created
//and used by buildTree
TwoThreeTree::InsertResult TwoThreeTree::insertHelper(const std::string& word, int line, node*& t){
    InsertResult result(false);

	//empty subtree: create new node here
	if(t == nullptr){
	    Key newKey(word,line);
		t = new node(newKey);
	    return result;
    }

	// Duplicate in first key
	if (word == t->first.word) {
		t->first.lines.push_back(line);
		return result;
	}

	// Duplicate in second key
	if (t->keyCount == 2 && word == t->second.word) {
		t->second.lines.push_back(line);
		return result;
	}

	// leaf cases
	if (isLeaf(t)) {
		// leaf with one key : insert in order
		if (t->keyCount == 1) {
			Key newKey(word,line);

			if (word < t->first.word) {
				t->second = t->first;
				t->first = newKey;
			}
			else {
				t->second = newKey;
			}

			t->keyCount = 2;
			return result;
		}

		// leaf with two keys : split the node
		Key small;
		Key middle;
		Key large;
		Key newKey(word, line);

		if (word < t->first.word) {
			small = newKey;
			middle = t->first;
			large = t->second;
		}
		else if (word < t->second.word) {
			small = t->first;
			middle = newKey;
			large = t->second;
		}
		else {
			small = t->first;
			middle = t->second;
			large = newKey;
		}

		t->first = small;
		t->keyCount = 1;

		node* newRight = new node(large);
		result.split = true;
		result.promotedKey = middle;
		result.rightChild = newRight;
		return result;
	}

	// Internal node cases
	if (t->keyCount == 1) {
		//recurse left
		if (word < t->first.word){
			InsertResult childResult = insertHelper(word,line, t->left);

			if (!childResult.split){
				return result;
			}

			// absorb into 1-key parent from left child split
			t->second = t->first;
			t->first = childResult.promotedKey;
			t->keyCount = 2;
			
			t->middle = childResult.rightChild;
			return result;
		}
		// recurse right
		else {
			InsertResult childResult = insertHelper(word, line, t->right);
			if (!childResult.split) {
				return result;
			}
			// absorb into 1 key parent from right child split
			t->second = childResult.promotedKey;
			t->keyCount = 2;
			t->middle = t->right;
			t->right = childResult.rightChild;

			return result;
		}
	}
	else {
		// 2 key parent, recurse left/middle/right
		if (word < t->first.word){
			InsertResult childResult = insertHelper(word,line,t->left);

			if (!childResult.split){
				return result;
			}

			// Left child split: keys are childResult.promotedKey, first, second
			Key promoteToParent = t->first;

			node* newRightNode = new node(t->second);
			newRightNode->left = t->middle;
			newRightNode->right = t->right;

			t->second = Key();
			t->first = childResult.promotedKey;
			t->keyCount = 1;
			t->right = childResult.rightChild;
			t->middle = nullptr;

			result.split = true;
			result.promotedKey = promoteToParent;
			result.rightChild = newRightNode;
			return result;
		}
		else if (word > t->second.word) {
			InsertResult childResult = insertHelper(word,line,t->right);

			if (!childResult.split){
				return result;
			}

			// right child split: keys are first, second, childResult.promotedKey
			Key promoteToParent = t->second;
			node* newRightNode = new node(childResult.promotedKey);
			newRightNode->left = t->right;
			newRightNode->right = childResult.rightChild;

			t->second = Key();
			t->keyCount = 1;
			t->right = t->middle;
			t->middle = nullptr;

			result.split = true;
			result.promotedKey = promoteToParent;
			result.rightChild = newRightNode;
			return result;
		}
		else {
			InsertResult childResult = insertHelper(word,line,t->middle);

			if (!childResult.split){
				return result;
			}

			// middle child split: keys are first, childResult.promotedKey, second
			Key oldSecond = t->second;

			node* newRightNode = new node(oldSecond);
			newRightNode->left = childResult.rightChild;
			newRightNode->right = t->right;

			t->second = Key();
			t->keyCount = 1;
			t->right = t->middle;
			t->middle = nullptr;

			result.split = true;
			result.promotedKey = childResult.promotedKey;
			result.rightChild = newRightNode;
			return result;
		}
	}
	return result;
}


//Used by contains() to see if a words is present or not. Will
//give contains() a pointer to the found node so that contains()
//can prints the lines the word was found on.
bool TwoThreeTree::containsHelper(const std::string& word, node* t, node*& result) const{
	if (t == nullptr) {
		return false;
	}

	if (word == t->first.word){
	    result = t;
 	    return true;
	}

	if (t->keyCount == 2 && word == t->second.word){
		result = t;
		return true;
	}

	if (t->keyCount == 1) {
		if (word < t->first.word){
			return containsHelper(word, t->left, result);
		}
		else {
			return containsHelper(word, t->right, result);
		}
	}
	else {
		if (word < t->first.word){
			return containsHelper(word, t->left, result);
		}
		else if (word > t->second.word) {
			return containsHelper(word, t->right, result);
		}
		else {
			return containsHelper(word, t->middle, result);
		}
	}
	return false;
}




//Called by printTree(), does the actual formatted printing
//in order traversal
void TwoThreeTree::printTreeHelper(node* t, std::ostream& out) const {
    if(t == nullptr) {
		return;
	}

	if (t->keyCount == 1){
		printTreeHelper(t->left, out);

		out << std::setw(30) << std::left;
		out << t->first.word << " " << t->first.lines[0];
		for (int i = 1; i < static_cast<int>(t->first.lines.size()); i++) {
			out << ", " << t->first.lines[i];
		}
		out << std::endl;

		printTreeHelper(t->right, out);
	}
	else {
		printTreeHelper(t->left, out);
		out << std::setw(30) << std::left;
		out << t->first.word << " " << t->first.lines[0];
		for (int i = 1; i < static_cast<int>(t->first.lines.size()); i++) {
			out << ", " << t->first.lines[i];
		}
		out << std::endl;

		printTreeHelper(t->middle, out);
		out << std::setw(30) << std::left;
		out << t->second.word << " " << t->second.lines[0];
		for (int i = 1; i < static_cast<int>(t->second.lines.size()); i++) {
			out << ", " << t->second.lines[i];
		}
		out << std::endl;

		printTreeHelper(t->right, out);
	}
}
		
//Returns height of tree. If tree has only one node, height is 1    
int TwoThreeTree::findHeight(node *t) const{
    if (t == nullptr) {
		return 0;
	}
    if (isLeaf(t)) {
		return 1;
	}

	if (t->keyCount == 1) {
		int leftHeight = findHeight(t->left);
		int rightHeight = findHeight(t->right);
		return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
	}

	else {
		int leftHeight = findHeight(t->left);
		int middleHeight = findHeight (t->middle);
		int rightHeight = findHeight (t->right);

		int maxHeight = leftHeight;
		if (middleHeight > maxHeight) {
			maxHeight = middleHeight;
		}
		if (rightHeight > maxHeight) {
			maxHeight = rightHeight;
		}

		return maxHeight + 1;
	}
}

//returns true if node is a leaf node
bool TwoThreeTree::isLeaf(node* t) const {
	return t != nullptr && t->left == nullptr && t->middle == nullptr && t->right == nullptr;
}
