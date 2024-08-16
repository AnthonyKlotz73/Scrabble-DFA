#ifndef SCRABLETREE_H
#define SCRABLETREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

/**
 * @brief this class will represent a single node in a tree represntation
 * of a DFA
*/
class Node {
public:
	/**
	 * @brief the default constructor for a node.
	 * Only used for the root of the tree
	*/
	Node() {
		parent = nullptr;
		value = '_';
		ifAcceptingState = false;
	}
	/**
	 * @brief Constructor for an average node
	 * @param nValue the character value for this node (a-z+)
	 * @param nAcc weather this node is an accepting state on creation
	 * @param nParent sence this is a tree data each node has to have a
	 * reference to its parent
	*/
	Node(char nValue, bool nAcc, Node* nParent) {
		parent = nParent;
		value = nValue;
		ifAcceptingState = nAcc;
	}
	/**
	 * @brief gets the value of this node
	 * @return the value of this node (char)
	*/
	char getValue() { return value; }
	/**
	 * @brief gets if it is an accepting state
	 * @return wheather the node is a accetingState, meaning that it is a valid
	 * stoping point (boolean)
	*/
	bool getIfAcceptingState() { return ifAcceptingState; }
	/**
	 * @brief gets the children of this node
	 * @return returns a vector of nodes representing the children of this node
	*/
	vector<Node*> getChildren() { return child; }
	/**
	 * @brief makes this node into a accepting state
	*/
	void makeAcceptingState() { ifAcceptingState = true; }
	/**
	 * @brief adds a node to the children
	 * @param currentChar is the char that the child will have
	 * @param nParent is a refrence to the childs parent
	*/
	void addNode(char currentChar, Node* nParent)
	{
		Node* temp = new Node(currentChar, false, nParent);
		child.push_back(temp);
	}
	/**
	 * @brief gets this nodes parrent for bottom up travercle
	 * @return the current nodes parent
	*/
	Node* getParent() {
		return parent;
	}
private:
	/**
	 * @brief is the parrent to the current node
	*/
	Node* parent;
	/**
	 * @brief the value that the current node has (char [a-z+])
	*/
	char value;
	/**
	 * @brief detemeands if the current node is an accepting sate (in refrence
	 * to a DFA or NFA style graph)
	*/
	bool ifAcceptingState;
	/**
	 * @brief a vector array of reffernces to node (to make the tree structure)
	*/
	vector<Node*> child;
};
/**
 * @brief this class will represent the graph of a DFA with ways to add and
 * validate words in the tree
*/
class S_Tree {
public:
	/**
	 * @brief Creates the root node as a default node and the nnumber of
	 * accepting states a 0
	*/
	S_Tree() {

		root = new Node();
		numberOfAcceptingStates = 0;

	}
	//level order travercle 
	/*
	void LevelOrderTracersal()
	{
		queue<Node*> q;
		q.push(root);
		while (!q.empty()) {
			int n = q.size();
			while (n > 0) {
				Node* p = q.front();
				q.pop();
				//cout << p->getValue() << " ";
				for (int i = 0; i < p->getChildren().size(); ++i) {
					q.push(p->getChildren()[i]);
				}
				n--;
			}
			//cout << endl;
		}
	}*/

	/**
	 * @brief goes throught the tree and adds nodes to the tree to add them to the tree
	 *
	 * @brief Checks if the word is too small. Then creates a queue if the word is
	 * greater that 1 length. This queue will be used to iterate through each
	 * character int the string. Will only pop if has found that char in the
	 * parent's children, then goes to the child. The queue will also pop if the
	 * current node does not have that char in its children
	 * @param neWord this is the word to be added
	 * @return an int
		*0: means that it was added with a new acceptingstate
		*1: means that the word was already part of a word
		*3: means that the word is too small
	*/
	int addWord(string neWord)
	{
		//too small of a word
		if (neWord.size() < 2) {
			return 3;
		}
		//cout << neWord << endl;
		queue<char> wordQueue;
		for (int i = 0; i < neWord.size(); ++i) {
			wordQueue.push(neWord.at(i));
		}
		bool addedToRow = false;
		Node* currentNode = root;
		while (!wordQueue.empty()) {
			addedToRow = false;
			char currentLetter = wordQueue.front();
			//cout << currentLetter;
			int index = -1;
			//a loop tha checks all the current childens nods if they have the current char
			for (int j = 0; j < currentNode->getChildren().size(); ++j) {
				if (currentLetter == currentNode->getChildren()[j]->getValue()) {
					//if so pop the the current letter
					index = j;
					addedToRow = true;
					wordQueue.pop();
					if ((j + 1) != currentNode->getChildren().size()) {
						if (!wordQueue.empty()) {
							currentLetter = wordQueue.front();
						}
					}
					break;
				}
			}
			if (!addedToRow) {
				//adds a child to the current node
				currentNode->addNode(currentLetter, currentNode);
				index = currentNode->getChildren().size() - 1;
				wordQueue.pop();
			}
			currentNode = currentNode->getChildren()[index];
			if (wordQueue.empty()) {
				//if the queue is empty (meaning the we have gone through the whole word) 
				//then detemen if the current node is a accepting state. if it isn't then
				//make it one. if it is then continue as normal
				if (currentNode->getIfAcceptingState()) {
					return 1;
				}
				else {
					currentNode->makeAcceptingState();
				}
			}

		}
		return 0;
	}
	/**
	 * @brief Checks to see if the word is in the tree
	 * @brief starts by converting the string to a queue. Then goes down the tree
	 * and check each nodes childern until the queue is empty. When empty the
	 * retruns weather the current node is an accepting state.
	 * @param checkWord the word to be check to see if it is a valid word.
	 * @return true if is a valid word in the current dictionary. False if the
	 * word is not in the current dictionary
	*/
	bool checkWord(string checkWord)
	{
		queue<char> wordQueue;
		for (int i = 0; i < checkWord.size(); ++i) {
			wordQueue.push(checkWord.at(i));
		}
		bool letterFound = false;
		Node* currentNode = root;
		while (!wordQueue.empty()) {
			letterFound = false;
			char currentLetter = wordQueue.front();
			for (int j = 0; j < currentNode->getChildren().size(); ++j) {
				if (currentLetter == currentNode->getChildren()[j]->getValue()) {
					///cout << "Approved\n";
					wordQueue.pop();
					letterFound = true;
					currentNode = currentNode->getChildren()[j];
					break;
				}
			}
			if (wordQueue.empty()) {
				return currentNode->getIfAcceptingState();
			}
			else if (!letterFound) {
				return false;
			}
		}

		return false;
	}
private:
	//the begining of the tree
	Node* root;
	// the number of accepting states
	int numberOfAcceptingStates;
};


#endif //SCRABLETREE_H