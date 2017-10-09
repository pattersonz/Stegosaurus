/***********************************************
 * This program takes in a short message and will
 * convert the bits of the text into word options
 * that will be placed in a fake messages. This
 * takes in 3 arguments
 * Output file - file to output fake message to
 * Message - the text to parse for the fake message
 * vars - the text with varying words to choose
 *
 * author: Zach Patterson & Austin Gordon
 * 10/4/2017
 ***********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

/*
 * this function reads in a group of options from the
 * vars file and selects the word to output based off
 * a binary number represented by the hidden message
 * --------PARAMS----------
 * numOptions - number of word options the vars file has
 * bitCount - the current bit that will be accessed
 * bits - array of bits
 * vars - input stream from vars
 * max - the number of bits in thbe bit array
 * @return - string to ouput to the message
 */
string readVars(int numOptions, int & bitCount, int * bits, istream& vars, int max)
{
	//log2 of options is the number of bits used
	int numBits = (int)log2(numOptions);

	//get the number of the option to be selected from the vars file
	int numSel = 0;

	//if the bit count is further than the total number of bits
	//append 0s
	for (int i = 0; i < numBits; i++)
		if (bitCount < max)
			numSel += bits[bitCount++] * pow(2, numBits - i - 1);

	//read until '{'
	char c = '1';
	while (c != '{')
		vars.get(c);


	string returnS = "";
	int numAsts = 0;
	//read until '}' store numSel in a string
	vars.get(c);
	while ( c != '}')
	{
		if (numAsts == numSel)
		{
			if (c != '*')
				returnS.push_back(c);
			else break;
		}
		if (c == '*')
			numAsts++;

		vars.get(c);
	}

	return returnS;
}

//main
int main( int argc, char *argv[])
{
	ofstream output; 
	ifstream message, vars; //3 files
	output.open( argv[1]); //first param, input
	message.open( argv[2]); //second param message text
	vars.open( argv[3]); //third param variable text

	//prompt for message and get message
	cout << "What is the *short* message you wish to hide?\n";
	string plainT;
	getline(cin, plainT);

	//convert message to bits
	int *bits = new int[plainT.length() * 7];
	for (int i = 0; i < plainT.length(); i++)
		for (int j = 0; j < 7; j++)
			bits[i*7 + j] = ((int)plainT[i] >> (6 - j)) % 2;  

	int bitCount = 0;


	//read in all chars from message
	char c;
	message.get(c);
	while ( ! message.eof())
	{

		//if char is { then it is start of vars
		if (c == '%')
		{
			int x;
			message >> x;
			output << readVars(x, bitCount, bits, vars, plainT.length() * 7);
		}
		else
			output << c; 

		message.get(c);
	}

	vars.close();
	message.close();
	output.close();

	return 0;
}