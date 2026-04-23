//Description: Takes a text file supplied by the user
//             and turns it into a word index, implemented
//             through the use of a BST 

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "bst.h"
#include "TwoThreeTree.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
	    std::cout << "Incorrect input. Correct format: ./<exectuable.out> <inputtext.txt>\n";
	    return 1;
	}

	std::string inputFileName = argv[1];
	char treeChoice;

	std::cout << "Options: (a) BST, (b) 2-3 Tree, (c) Compare BST and 2-3 Tree\n";
	std::cin >> treeChoice;

	if (treeChoice == 'a') {
		std::ifstream input(inputFileName.c_str());
		BST myTree;
		int choice = 0;

		if (!input.is_open()) {
			std::cout << "Invalid File Name. Restart Program.\n";
			return 2;
		}

		myTree.buildTree(input);
		input.close();

		while (1) {
			choice = 0;
			std::cout << "Options: (1) display index, (2) search, (3) save index, (4) quit\n";
			std::cin >> choice;

			if (choice == 1) {
				myTree.printTree(std::cout);
			}

			else if (choice == 2) {
				myTree.contains();
			}
			else if (choice == 3) {
				std::string outputFile;
				std::cout << "Enter a filename to save your index to : ";
				std::cin >> outputFile;
				std::ofstream output(outputFile.c_str());

				if (!output.is_open()) {
					std::cout << "Could not open file.\n";
				}
				else {
					myTree.printTree(output);
					output.close();
					std::cout << "Saved\n";
				}
			}
			else {
				break;
			}
		}
	}
	else if (treeChoice == 'b') {
		std::ifstream input(inputFileName.c_str());
		TwoThreeTree myTree;
		int choice = 0;

		if (!input.is_open()) {
			std::cout << "Invalid File Name. Restart Program.\n";
			return 2;
		}

		myTree.buildTree(input);
		input.close();

		while (1){
			choice = 0;
			std::cout << "Options: (1) display index, (2) search, (3) save index, (4) quit\n";
			std::cin >> choice;

			if (choice == 1) {
				myTree.printTree(std::cout);
			}

			else if (choice == 2) {
				myTree.contains();
			}

			else if (choice == 3) {
				std::string outputFile;
				std::cout << "Enter a filename to save your index to : ";
				std::cin >> outputFile;
				std::ofstream output(outputFile.c_str());
				if (!output.is_open()) {
					std::cout << "Could not open file.\n";
				}
				else {
					myTree.printTree(output);
					output.close();
					std::cout << "Saved\n";
				}
			}
			else {
				break;
			}
		}
	}
	else if (treeChoice == 'c') {
        std::ifstream bstInput(inputFileName.c_str());
        std::ifstream ttInput(inputFileName.c_str());
        BST bstTree;
        TwoThreeTree ttTree;

        if (!bstInput.is_open() || !ttInput.is_open()) {
            std::cout << "Invalid File Name. Restart Program.\n";
            return 2;
        }

        bstTree.buildTree(bstInput);
        ttTree.buildTree(ttInput);
        bstInput.close();
		ttInput.close();

		auto wordsSet = bstTree.dumpTree();

		auto bstStart = std::chrono::high_resolution_clock::now();
		for(const std::string& word : wordsSet){
			bstTree.contains(word);
		}
		auto bstEnd = std::chrono::high_resolution_clock::now();

		auto ttStart = std::chrono::high_resolution_clock::now();
		for(const std::string& word : wordsSet){
			ttTree.contains(word);
		}
		auto ttEnd = std::chrono::high_resolution_clock::now();

		auto bstDuration = std::chrono::duration_cast<std::chrono::microseconds>(bstEnd-bstStart);
		auto ttDuration = std::chrono::duration_cast<std::chrono::microseconds>(ttEnd-ttStart);

		std::cout << "BST Time to search each item in the table: " << bstDuration.count() << " microseconds\n";
		std::cout << "Two-Three Time to search each item in the table: " << ttDuration.count() << " microseconds\n";

	}
	else {
		std::cout << "Invalid option.\n";
	}
	return 0;
}
