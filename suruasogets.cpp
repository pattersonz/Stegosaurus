/* Description: This program takes a fake message from a file and extracts the hidden message from it. This is possible
 *	because various options of the fake can be reconstructed as binary bits, that when added all together will
 *	represent the original message.
 *
 * Arguements: Requires 2 files names from the cmd line when calling. The file that contains the fake, and a file to
 *	output the decoded results to.
 *
 * Authors: Austin Gordon & Zach Patterson
 *
 * Last Modified: 10/5/17
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

int getOptions(string *options, istream &source);

int main(int argc, char *argv[]) {
  //I/O vars
  ifstream fake;		//stream for input of encoded fake
  ifstream constant, var;	//streams for the constant and variable portions of fake template
  //processing vars
  char c;	//stores chars for stream inputting
  int x;	//get number of options
  string encodedText;	//the entire fake message
  string *options;	//the current set of options, 8 max
  size_t iterator;	//where we are in the encoded fake
  int largest;		//longest option
  int *binary;	//stores the binray of the secret message
  int binPos = 0;       //the current binary position 
  string plainText;	//stores the decoded message

  //Init streams
  fake.open(argv[1]);
  constant.open(argv[2]);
  var.open(argv[3]);

  //count number of bits the message can hold
  int numBits = 0, mostOptions = 0;
  while (constant.get(c))
  {
    // if % was found, following is a number of bits
    if ( c == '%')
    {
      int x, logV = 0;
      //read in int
      constant >> x;
      if ( x > mostOptions)
        mostOptions = x;
      //calculate log 2
      while (x >>= 1)
        logV++;
      //add number of bits to total
      numBits += logV;
    }
  }  
  //set size of binary array
  binary = new int [numBits];
  options = new string [mostOptions];

  //restart the constant stream
  constant.close();
  constant.open(argv[2]);


  //read entire fake into string
  encodedText = "";
  while (fake.get(c)) {
    encodedText.push_back(c);
  }

  iterator = 0;
  //read through constant template until we see an option was made
  while (constant.get(c)) {
    if (c == '%') {	//% marks start of option
      largest = getOptions(options, var);	//so get the options
      constant >> x;	//this will be the number of options
      //search fake for one of the options
      for (unsigned int i = 0; i < x; i++) {
      	int r = encodedText.find(options[i], iterator);
      	if ((r != -1) && (r == iterator)) {	//we have found a match
      	  for (int j = log2(x); j > 0; j--) {
      	    binary[binPos] = (i >> (j - 1)) % 2;	//add binary value of match
      	    binPos++;
      	  }
      	  iterator += options[i].length();
      	  break;
      	} 
      }
    } else {
      iterator ++;
      //do nothing, we only need option values at this point
    }
  }

  for (int i = 0; i < binPos; i++) {
    cout << binary[i];
  }
  cout << endl;

  //convert binary into plaintext message and output
  for (int i = 0; i < binPos; i+= 7) {
    c &= 0;
    for (int j = 0; j < 7; j++)
      c += binary[i + j] << (6 - j);
    cout << c;
  }
  cout << endl;

  //close streams and exit
  fake.close();
  constant.close();
  var.close();
  return 0;
}


/* Description: This function reads in and separates options from the vars file.
 *
 * @Params
 * options: A string array to store the options
 * source: Input stream to get options from
 *
 * @Returns
 * Fills out the options array.
 */
int getOptions(string *options, istream &source) {
  char c;	//char for input stream
  int currentIndex = 0;		//which index are we on in the array/set
  string currentOption = "";	//the actual string that is the current option
  int longest, cur = 0;	//used to find max

  while (source.get(c)) {	//read through entire set of options
    if (c == '{') {
      //just ignore this char
    } else if (c == '*') {	//this is the end of the option
      options[currentIndex] = currentOption;	//assign full string to array
      currentOption = "";	//reset for next
      currentIndex++;
      if (cur > longest) {
	longest = cur;
      }
    } else if (c == '}') {	//this is the end of the set of options
      options[currentIndex] = currentOption;	//assign full string to array
      source.ignore(10, '\n');	//flush the last newline and break
      if (cur > longest) {
	longest = cur;
      }
      break;
    } else {	//text of an option
      currentOption.push_back(c);	//add to string of the current option
      cur++;
    }
  }
  return longest;
}
