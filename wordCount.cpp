/*	File:	wordCount.cpp
 *	Author:	Connor Leslie
 *	Class: CS315
 *	Date 4/14/21
 *	Discription:
 *		The program takes in input from standard input, counts the occurance of words, 
 *		and prints them both alphabetically [ascii] and by occurance
 */

#include <string>
#include <iostream>
#include <vector>
using namespace std;

//The size of the hashTable array
#define CAPACITY 8192

//Array of Puncuations to remove from the given input along with the given length
const int PUNCLEN = 11;
const char PUNC[] ={'.', '\'', '!', '?', '(', ')', ';', ',', '-', '`', '\"'}; 



//Node that holds the amount of times a word is found and what the word is
struct hashNode{
	unsigned int amount;
	string word;
};

//makes a node pointer that sets amount to 1 and holds the given word
hashNode makeNode(string key){
	hashNode* newNode = new hashNode();
	hashNode node = *newNode;
	node.amount = 1;
	node.word = key;
	return node;
}

//returns the corresponding hashtableValue for the given word
unsigned int hashValue(string key){
	unsigned int value = 0;
	for(long unsigned int i = 0; i < key.size(); i++){
		value +=( (unsigned int)key.at(i) * (i+1) );
	}
	value %= CAPACITY;
	return value;
}

//partitions given list for QuickSort on the word Value of hashNode
int partitionOnWord(vector<hashNode> &list, int left, int right){
	hashNode pivot = list.at(right);
	int index = left;
	hashNode temp = *(new hashNode());

	for(int i = left; i < right; i++){
		if(list.at(i).word.compare(pivot.word) < 0){
			//swap
			temp = list.at(index);
			list.at(index) = list.at(i);
			list.at(i) = temp;

			index++;
		}
	}
	//swap 
	temp = list.at(right);
	list.at(right) = list.at(index);
	list.at(index) = temp;
	return index;
}

//sort list on the Word value of the hashNodes
void quickSortOnWord(vector<hashNode> &list, int left, int right){
	if(left < right){
		int splitIndex = partitionOnWord(list, left, right);
		quickSortOnWord(list, left, splitIndex -1);
		quickSortOnWord(list, splitIndex+1, right);
	
	}
}

//wrapper function that sorts entire list on the word value of the hashNodes
void sortOnWord(vector<hashNode> &list){
	quickSortOnWord(list, 0, list.size()-1);
}

//partitions list for quickSort on the amount value of the hashNodes
int partitionOnAmount(vector<hashNode> &list, int left, int right){
	hashNode pivot = list.at(right);
	int index = left;
	hashNode temp = *(new hashNode());

	for(int i = left; i < right; i ++){
		if(list.at(i).amount > pivot.amount){
			//swap
			temp = list.at(index);
			list.at(index) = list.at(i);
			list.at(i) = temp;

			index++;
		}
	}
	//swap
	temp = list.at(right);
	list.at(right) = list.at(index);
	list.at(index) = temp;
	return index;
}

//sort list on the amount value of the hashNodes
void quickSortOnAmount(vector<hashNode> &list, int left, int right){
	if(left < right){
		int splitIndex = partitionOnAmount(list, left, right);
		quickSortOnAmount(list, left, splitIndex - 1);
		quickSortOnAmount(list, splitIndex +1, right);
	}
}

//wrapper function that sorts entire list on the amount value of the hashNodes
void sortOnAmount(vector<hashNode> &list){
	quickSortOnAmount(list, 0 , list.size()-1);
}





//add one to the node that corresponds with given key. Return 1 if found return -1 on failure
int addOne(vector<hashNode> &vec, string key){
	for(long unsigned int i = 0; i < vec.size(); i ++){
		if(vec.at(i).word.compare(key) == 0){//node match with key is found
			vec.at(i).amount ++;
			return 1;
		}
	}
	return -1;
}


//clean the ends of the given string by removing all punctutation appendages
//return -1 if the string is empty and 1 if the execution is successful
int endPuncClean(string &key){

	if(key.size() == 0){//the string is empty
		return -1; 
	}
	for(int i =0; i < PUNCLEN; i++){
		if(key.at(0) == PUNC[i]){//if there is a PUNC at the first index remove it and call again
			key= key.substr(1, key.size()-1);
			return endPuncClean(key);
		}
		if(key.at(key.size()-1) == PUNC[i]){//if there is a PUNC at the last index remove it and call again
			key = key.substr(0, key.size()-1);
			return endPuncClean(key);
		}
	}
	//ended with a string
	return 1;

}

//clean the given string and put individual words seperated by characters in PUNC into
//given array
//only applicable after endPuncClean
void middlePuncClean(vector<string> &list, string &input){
	int indexFound = -1;
	//loop through all PUNC
	for(int i = 0; i < PUNCLEN; i ++){
		indexFound = input.find(PUNC[i]); //find instence of the punctuation
		if(indexFound >0){
			//split input into two strings,add completed string to list, and recursively call on unchecked string
			list.push_back(input.substr(0, indexFound));
			input = input.substr(indexFound+1, input.size()-indexFound-1);
			middlePuncClean(list, input);
		}
	}
	//put last clean string into list
	list.push_back(input);
}

//print the values of the hashNodes in a list
void print(vector<hashNode> list){
	for(long unsigned int i = 0; i < list.size(); i++){
		cout<< list.at(i).word << " " << list.at(i).amount << endl;
	}
}




int main(){
	vector<hashNode> hashTable[CAPACITY];
	string key;// inputted value
	unsigned int index;// value of the hash for key
	vector<string> inputs;//holds the words in key
	string iterativeInput;//the current word being worked on
	while(cin >> key){//run until cin stops
		if(endPuncClean(key)>0){ //if Cleaning the ends of the inputted string is not ""
			//clean middle punc
			middlePuncClean(inputs, key);
			
			for(long unsigned int i = 0; i < inputs.size(); i++){//for every word found in the input
				iterativeInput = inputs.at(i);	
				index = hashValue(iterativeInput);
				if(addOne(hashTable[index], iterativeInput)==-1){//if addOne is unsuccessful make a node and add it
					hashNode newNode = makeNode(iterativeInput);
					hashTable[index].push_back(newNode);
				}
			
				
			}


		

		}
		inputs.clear();//clear inputs for future iterations
	}


	//create list of all nodes
	vector<hashNode> list;
	for(int j = 0; j < CAPACITY; j++){

		for(long unsigned int k = 0; k < hashTable[j].size(); k++){
			list.push_back(hashTable[j].at(k));
		}
	}

	//sort alphabetically
	sortOnWord(list);
	//print
	print(list);
	cout << endl;
	//sort numerically
	sortOnAmount(list);
	//print
	print(list);
	

	//fin
	return 0;
}
