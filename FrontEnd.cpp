#include <fstream>
#include <string>
#include <regex>
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include "ScrabbleTree.h"
using namespace::std;
/**
* This is a projec that i worked on to try an make a scrabble dictionary.
* and to do this i wande to use some cool data stuctures and language tools
* in this project i replicated a DFA/NFA to simulate a dictionary for the game scrabble
* to acheive this i used a tree to replicate the DFA/NFA created here.
* On each load it will read though the given text document dictionary.txt
* this file can be eddited if required.
* @author Anthony Klotz -2023
/
/**
 * @brief this struct is used to help withidentifying spaific words for choice 4
 * such as when you have _a__ and want the most point value words given the chars you have
*/
struct specificWord {
	specificWord(string x, int y) {
		word = x;
		place = y;
	}
	/**
	 * @brief is the letter in a word
	*/
	string word = "";
	/**
	 * @brief that letters place in a word
	*/
	int place = -1;
};

/**
 * @brief this class is used for the main front end of this program - refactored by Anthony Klotz
*/
class FrontEnd {
public:
	bool readFromAFile(ifstream&, string);
	string toLower(string word, int index);
	bool isValid(regex, string);
	//void printWordsF(vector<string>&);
	S_Tree NFA;
	
	int getPoints(string letters);
	vector <string> checkword;
	vector<int> checkvalue;
	void diffrentFlagPermutation(int x, vector<string> arr);
	void specialPermutaion(int maxWordLength, vector<specificWord>, vector<string>arr);
	bool specialConditionMet(vector <specificWord> validation, string word);
};
struct wordsValue {
	wordsValue(string x, int y) {
		word = x;
		score = y;
	}
	string word = "";
	int score = -1;
};

string menu();
int main()
{
	FrontEnd termonal;
	vector<string> checkingwords;
	ifstream testFile;
	string fileName;
	string findword;
	regex aphaOnly("[a-z]+");
	regex numbOnly("[1-9]+");
	//vector<string> library;//vaild words
	termonal.readFromAFile(testFile, "dictionary.txt");
	int choice;
	string sChoice;
	string testWord;
	bool loop = true;
	while (loop) {
		choice = -1;
		cout << menu();
		cin >> sChoice;
		if (termonal.isValid(numbOnly, sChoice)) {
			choice = stoi(sChoice);
		}
		if (choice == 0)
		{
			system("cls");
		}
		else if (choice == 1)
		{
			cout << "Please enter a file name (Text.txt): ";
			cin >> fileName;
			if (!termonal.readFromAFile(testFile, fileName)) {
				cout << "try again\n\n";
				//exit(1);
			}
			//termonal.printWordsF(library);
			//printValidStrings(library, 0);			
		}
		else if (choice == 2)
		{	// Asks the user for the letters they have, recusive for loop to create every possibility with the letters
			cout << "Please enter a word to add it to the dictionary\n---->";
			cin >> testWord;
			termonal.NFA.addWord(testWord);
		}
		else if (choice == 3)
		{

			cout << "Please enter the letters you would like to find a word for: ('abc' not 'a b c' or 'a,b,c'): ";
			cin >> findword;
			regex aphaOnly("[a-z]+");
			if (termonal.isValid(aphaOnly, findword)) {
				vector<string> charVec;
				char a = findword[0];
				for (int i = 0; i < findword.size(); ++i) {
					string s(1, findword[i]);
					charVec.push_back(s);
				}
				cout << "******Valid Words******\n";
				termonal.diffrentFlagPermutation(charVec.size(), charVec);
				cout << "***********************\n";
			}
			else {
				cout << "Invalid Input";
			}
		}
		else if (choice == 4) {
			vector<specificWord> sWords;
			string numOfSpecificS = "a";
			while (termonal.isValid(numbOnly, numOfSpecificS)) {
				cout << "Please enter the number of speicific letters: ";
				cin >> numOfSpecificS;
			}
			int numOfSpecific = stoi(numOfSpecificS);
			string maxLengthS = "a";
			while (termonal.isValid(numbOnly, numOfSpecificS)) {
				cout << "Please enter the maximum length the word can be:";
				cin >> maxLengthS;
			}
			int wordLength = stoi(maxLengthS);
			for (int i = 0; i < numOfSpecific; ++i) {
				int position = -1;
				string letter;
				cout << to_string(i) << ") Please enter the position of the spefication: ";
				cin >> position;
				cout << to_string(i) << ")Please enter the letter at this position: ";
				cin >> letter;
				letter = letter[0];
				specificWord temp = specificWord(letter, /*wordLength - */position - 1);
				sWords.push_back(temp);
			}
			for (int i = 0; i < sWords.size(); ++i) {
				cout << sWords[i].word << " at " << sWords[i].place << endl;
			}
			cout << "Please enter the letters you have: ('abc' not 'a b c' or 'a,b,c'): ";
			cin >> findword;
			regex aphaOnly("[a-z]+");
			if (termonal.isValid(aphaOnly, findword)) {
				vector<string> charVec;

				char a = findword[0];
				for (int i = 0; i < findword.size(); ++i) {
					string s(1, findword[i]);
					charVec.push_back(s);
				}
				for (int i = 0; i < sWords.size(); ++i) {
					cout << sWords[i].word << endl;
					charVec.push_back(sWords[i].word);
				}
				termonal.specialPermutaion(wordLength, sWords, charVec);
			}
			//testMonal.NFA.hopcrofts();
		}
		else if (choice == 5) {
			cout << "Please enter the a word to see if it is a valid word: ";
			cin >> findword;
			regex aphaOnly("[a-z]+");
			if (termonal.isValid(aphaOnly, findword)) {
				string outputMessage = "\n" + findword;
				if (termonal.NFA.checkWord(findword)) {
					outputMessage = outputMessage + ": is a valid word. It is worth " + to_string(termonal.getPoints(findword)) + " points\n";
				}
				else {
					outputMessage = outputMessage + " is not a vaild word.\n";
				}
				cout << outputMessage << endl;
			}
			else {
				cout << "\nNot a valid entry (a word with only a-z characters)\n" << findword << " does not meet this criteria\n\n";
			}
		}
		else
		{
			//quit the program
			exit(1);
		}
		//level order thing
		//termonal.NFA.LevelOrderTracersal();
	}
}
/**
 * @brief a simple method that can be eddited very quicly to add new options/features 
 * @return a string that represents the menu options for this program
*/
string menu() {
	return "0) Clear the Screen\n1) Add words from a file\n2)Add a word from the console\n3) Words that can be built from chars\n4) Check if you can make a word with conditions\n5) Check if word is valid\nAny other input will termonate the porgram\nEnter your selection--->";
}

/**
 * @brief reads in the words from a string with a loading bar
 * this fucntion will run threw the file two times, once to find all the valid
 * words in the string. the second time adds them to the tree.the first time
 * is a palimanry read to make use of a visual indiaction of how far the
 * program has made it through the file. File must be in the same directorty
 * @param fileStream: This is our file stream that will let us read form a file
 * @param fileName: This is the file name for the strings
 * @returns true if the file was able to be read, false if not able to be read
*/
bool FrontEnd::readFromAFile(ifstream& fileStream, string fileName) {
	//TODO program file input
	string tempString;
	regex aphaOnly("[a-z]+");
	int i = 0;
	int reading = 0;
	queue<int> invalidWords;
	auto start_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();
	fileStream.open(fileName);
	if (fileStream.fail()) {
		cout << "\nFile failed to open" << fileName << endl;
		return false;
	}
	else {
		try {
			cout << "************Invalid*Strings***********\n";
			start_time = std::chrono::high_resolution_clock::now();
			while (!fileStream.eof()) {
				getline(fileStream, tempString);
				tempString = toLower(tempString, 0);
				if (!isValid(aphaOnly, tempString)) {
					invalidWords.push(i);
					cout << tempString << endl;
				}
				i += 1;
			}
			current_time = std::chrono::high_resolution_clock::now();
		}
		catch (...) {
			cout << "Unable to read from file: " << fileName << endl;
			return false;
		}
	}
	fileStream.close();
	cout << "**************************************\n" << i << ": done in "
		<< std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count()
		<< " seconds.\n";
	fileStream.open(fileName);
	if (fileStream.fail()) {
		cout << "File failed to open\n";
		return false;
	}
	else {
		try {
			start_time = std::chrono::high_resolution_clock::now();
			int totalWords = i / 10;
			while (!fileStream.eof()) {
				bool skip = false;
				if ((reading % totalWords) == 0) {
					cout << " * ";
				}
				getline(fileStream, tempString);
				tempString = toLower(tempString, 0);
				if (!invalidWords.empty()) {
					if (invalidWords.front() == reading) {
						skip = true;
					}
				}
				if (!skip) {
					NFA.addWord(tempString);
				}
				reading += 1;
			}
			fileStream.close();
			current_time = std::chrono::high_resolution_clock::now();
			cout << endl << i << ": done in " << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() << " seconds.\n";
		}
		catch (...) {
			cout << "Unable to read from file: " << fileName << endl;
			return false;
		}
	}

	return true;
}
/**
 * @brief given a regex epersion checks if the string follows these rules
 * @param regularExpresion: is a regular expersion to check if the string follows its rules1
 * @param word: is the word that you would like to check given the regular expersion 
 * @return 
*/
bool FrontEnd::isValid(regex regularExpresion, string word) {
	return regex_match(word, regularExpresion);
}
/**
 * @brief a recureive fuction that converst strings to theri lower case version
 * @param word the string to be coverted to lower case
 * @param index where in the string this iteration is at in the word
 * @return a finished string that will be now lower case
*/
string FrontEnd::toLower(string word, int index) {
	if (index != word.length()) {
		//recursively
		if (index == NULL || index < 0) {
			index = 0;
		}
		word[index] = tolower(word.at(index));

		return toLower(word, index + 1);
	}
	return word;
}


//01 point A,E,I,O,U,L,N,S,T,R
//02 point D,G
//03 point B,C,M,P
//04 point F,H,V,W,Y
//05 point K
//08 point J,X
//10 point Q,Z


/**
 * @brief given a string, the fuction will run through the word and add up each letters value
 * @param letters the word that you would like to see how much it is worth
 * @return the point that that word would be rewarded in scrabble
*/
int FrontEnd::getPoints(string letters)
{
	string temp;
	int points = 0;
	for (int i = 0; i < letters.length(); i++)
	{
		if (letters[i] == 'd' || letters[i] == 'g') { points += 2; }
		else if (letters[i] == 'b' || letters[i] == 'c' || letters[i] == 'm' || letters[i] == 'p') { points += 3; }
		else if (letters[i] == 'f' || letters[i] == 'h' || letters[i] == 'v' || letters[i] == 'w' || letters[i] == 'y') { points += 4; }
		else if (letters[i] == 'k') { points += 5; }
		else if (letters[i] == 'j' || letters[i] == 'x') { points += 8; }
		else if (letters[i] == 'q' || letters[i] == 'z') { points += 10; }
		else { points += 1; }
	}

	return points;
}

/**
 * @brief gieven the letters that you pass in will print to the screen the valid words that can be played given those letters 
 * @param X the lenght of the given words to be printed to the screen
 * @param arr truely a vector of chars but was initaly wrote with strings
 * -Revised by Anthony Klotz
*/
void FrontEnd::diffrentFlagPermutation(int X, vector<string> arr)
{
	vector<string> ml;
	vector<wordsValue> validWords;
	ml = arr;


	int count = ml.size();
	// Traverse all possible lengths 
	for (int z = 0; z < X - 1; z++)
	{

		// Stores all combinations 
		// of length z 
		vector<string> tmp;

		// Traverse the array 
		for (int i = 0; i < arr.size(); i++)
		{
			for (int k = 0; k < ml.size(); k++)
			{
				if (arr[i] != ml[k])
				{

					// Generate all 
					// combinations of length z 
					tmp.push_back(ml[k] + arr[i]);
					count += 1;
				}
			}
		}
		// Print all combinations of length z 
		for (int i = 0; i < tmp.size(); i++)
		{
			if (NFA.checkWord(tmp[i])) {
				cout << tmp[i] << endl;
				wordsValue a = wordsValue(tmp[i], getPoints(tmp[i]));
				validWords.push_back(a);
			}
		}
		ml = tmp;
	}
	for (int o = 0; o < validWords.size(); ++o) {
		cout << validWords[o].score << " " << validWords[o].word << endl;
	}
	validWords.clear();
	ml.clear();
}
/**
 * @brief Given a situatio of puting letters in a spasicic spot, this fucntion
 * will print out the words given these specifations such as (a__l_)
 * @param X lenght of a possible word
 * @param specifics putting the a vector that has the stuct specificWords (a,0) meaning that at position 1 in the word the value should be a
 * @param arr the letters that can be used here
*/
void FrontEnd::specialPermutaion(int X, vector<specificWord> specifics, vector<string> arr)
{
	vector<string> ml;
	vector<wordsValue> validWords;
	ml = arr;


	int count = ml.size();
	// Traverse all possible lengths 
	for (int z = 0; z < X - 1; z++)
	{

		// Stores all combinations 
		// of length z 
		vector<string> tmp;

		// Traverse the array 
		for (int i = 0; i < arr.size(); i++)
		{
			for (int k = 0; k < ml.size(); k++)
			{
				if (arr[i] != ml[k])
				{

					// Generate all 
					// combinations of length z 
					tmp.push_back(ml[k] + arr[i]);
					count += 1;
				}
			}
		}
		// Print all combinations of length z 
		for (int i = 0; i < tmp.size(); i++)
		{
			if (NFA.checkWord(tmp[i])) {
				//checks if the speceial condition is met
				if (!specialConditionMet(specifics, tmp[i])) {
					wordsValue a = wordsValue(tmp[i], getPoints(tmp[i]));
					validWords.push_back(a);
				}

			}
		}
		ml = tmp;
	}
	for (int o = 0; o < validWords.size(); ++o) {
		cout << validWords[o].score << " " << validWords[o].word << endl;
	}
	validWords.clear();
	ml.clear();

}
/**
 * @brief the true diciding factor on wheather a word is valid in the
 * specialPermutaion fucntion. Checks at the given specications if they are in
 * the correct position givent the random permutaions from the function
 * @param validation the parramitors given to see if the word is valid
 * @param word the word to be checked
 * @return true if the word follows the rules given by the permuation.
 * false if not
 * 
*/
bool FrontEnd::specialConditionMet(vector<specificWord> validation, string word)
{
	int flag = 0;//number of times flagged
	for (int i = 0; i < validation.size(); ++i) {
		flag += (word[validation[i].place] == validation[i].word[0]);
	}
	return flag == 0;
}






