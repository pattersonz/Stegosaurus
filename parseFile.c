/***********************************************
 * This program splits a spam message template
 * into separate files. The code requires three
 * arguements.
 *
 * The first one is the file to read, which
 * should contain the full spam message. The
 * second is the file to write the constant
 * portions of the message to. The third is the
 * file to write the varying words of the spam
 * to.
 *
 * The program will parse through the first file
 * assuming a required format:
 * This_is_unvarying_spam {option1*option2*o3*o4}
 *
 * NOTE: number of options in a {} must be base 2
 * 		ie: 2,4,8,16....2^n
 *
 * The constant portions of text is written to
 * the second argument in the following format
 * given the above text:
 * This_is_varying_spam %4
 *
 * The variable portion will be output to the third
 * argument with one set of options per line:
 * {option1*option2*o3*o4}
 *
 * author: Zach Patterson
 * 10/4/2017
 ***********************************************/
#include <stdlib.h>
#include <stdio.h>

/*
 * take character inputs from input and place the
 * text in the vars file, the * are counted to 
 * determine the number of options the message
 * can have
 */
void readVars(FILE *input, FILE *message, FILE *vars)
{
	fprintf(vars, "{");

	char c = '{';
	int numAst = 0;
	//read input, add to counter if *
	while (c != '}')
	{
		c = fgetc(input);
		fprintf(vars, "%c", c);
		if (c == '*')
			numAst++;
	}
	fprintf(vars, "\n");
	fprintf(message, "%%%d", numAst + 1);
}

//main
int main( int argc, char *argv[])
{
	FILE *input, *message, *vars; //3 files
	input = fopen( argv[1], "r" ); //first param, input
	message = fopen( argv[2], "w" ); //second param message text
	vars = fopen( argv[3], "w" ); //third param variable text

	char c = 'a'; //character to read in and out

	//read in all chars
	while ( (c = fgetc(input)) != EOF)
	{
		//read in char

		//if char is { then it is start of vars
		if (c == '{')
			readVars(input, message, vars);
		else
			fprintf(message, "%c", c); 

	}

	return 0;
}
