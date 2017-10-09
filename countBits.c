/***********************************************
 * This program reads in the message_constant
 * file and reads the number of bits to confirm
 * if it is divisible by 7. Since all messages
 * are made of chars with 7 significant bits
 * all messages should be divisible to protect
 * the integrity of the message. This program
 * takes one parameter which is the name of the
 * file where the bits will be counted. This will
 * also count the number of characters that can
 * be fit in the message.
 *
 * author: Zach Patterson
 * 10/5/2017
 ***********************************************/
#include <stdlib.h>
#include <stdio.h>

/*
 * At this point, a % has been found, so an int
 * will be read in and used to determine the #
 * of bits this affects.
 */
int countbits(FILE *input)
{
	int x, logV = 0;
	fscanf(input, "%d",&x);
	while (x >>= 1)
		logV++;
	return logV;
}

//main
int main( int argc, char *argv[])
{
	FILE *input; //3 files
	input = fopen( argv[1], "r" ); //first param, input
	int bits = 0;

	char c = 'a'; //character to read in and out

	//read in all chars
	while ( (c = fgetc(input)) != EOF)
	{
		//read in char

		//if char is { then it is start of vars
		if (c == '%')
			bits += countbits(input);

	}
	if (bits % 7 == 0)
	{
		printf("this fake message considers: ");
		printf("%d-bits.\nthis is %d characters\n",bits, bits/7);
	}
	else
	{
		printf("your message does not consider a fitting number of bits\n");
		printf("it takes %d-bits, you need to add %d more.\n", bits, 7 - bits % 7);
	}

	return 0;
}
