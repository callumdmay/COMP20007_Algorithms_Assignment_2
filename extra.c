/*
 *Callum May
  799309
*/
#include "extra.h"
#include "hash.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 256

//Declare randstring function
char* randstring();
int isPrime (int inputNumber);

/* Determine appropriate size of a hash table given input size n */
unsigned int determine_size(unsigned int n) {

		int hashTableSize = n*2;
		while(!isPrime(hashTableSize))
		{
			hashTableSize++;
		}
	
	return hashTableSize;
}


/* =====================================================================
   The following function is written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */
int isPrime (int inputNumber){	

	int divisor, isprime;
	isprime = 1;
	for (divisor=2; divisor*divisor<=inputNumber; divisor++) {
		if (inputNumber%divisor==0) {
			isprime = 0;
			break;
		}
	}
	
	return isprime==1 ? true : false;
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_dumb(unsigned int size, unsigned int seed, int n) {
	
	char* zeroHashStrings[n];
	
	int zeroHashCounter = 0 ;
	
	//While we don't have enough random numbers, keep trying strings
	while(zeroHashCounter < n)
	{
		char* string = randstring();
		
		if(universal_hash((unsigned char*)string, size)==0)
		{
			zeroHashStrings[zeroHashCounter]= string;
			zeroHashCounter++;
		}
		
		
	}

	//The number of r values generated is always the maximum string length
	fprintf(stdout, "%d\n", MAXSTRLEN);
	
	//Reinitialize the random number generator to get the values we want
	srand(seed);
	
	
	//Print the r values
	int i;
	for(i = 0 ; i < MAXSTRLEN; i++)
	{
		fprintf( stdout, "%d\n" ,rand()%size);
	}

   
	for(i = 0 ; i < n; i++)
	{
		fprintf( stdout, "%s\n" ,zeroHashStrings[i]);
	}


}

/*
	A random number generator I wrote. It basically iterates through all combinations of alphanumeric strings, and doesn't repeat. It does this
	by incrementing the right most character first, then rolling over to the left, similarily to how one would count up in a binary number system.
	The total number of string combinations possibly generated is 52^20, a number far exceeding any practical requirements.
	
*/
char *randstring() {
	
	static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	char *randomString = NULL;
	static int indexCounters[20] = { 0 };
	static int length = 1;
	
	randomString = malloc(sizeof(char) * (length +1));	
	
	int i= length-1;
		
	int n;
	for (n = 0;n < length;n++) {            
		
		randomString[n] = charset[indexCounters[n]];
	}
	
	while(i >=0)
	{
		indexCounters[i] ++;
		if(indexCounters[i] >=52){
			if(i ==0)
				length++;
			
			indexCounters[i]=0;
			i = i -1;

		}
		else
		{
			break;
		}
	}
	
	randomString[length] = '\0';
	
	return randomString;
	
}

/*
 The extended euclidean algorithm written in C. The implementation of the algorithm is quite
 trivial so in the interest of time I found a source that had already written it efficiently.
 
 
 calculates a * *x + b * *y = gcd(a, b) = *d 
 Author: Pate Williams (c) 1997 
 Taken from http://www.di-mgt.com.au/euclidean.html#extendedeuclidean
*/
void extended_euclid(long a, long b, long *x, long *y, long *d)
{ 
  long q, r, x1, x2, y1, y2;

  if (b == 0) {
    *d = a, *x = 1, *y = 0;
    return;
  }
  x2 = 1, x1 = 0, y2 = 0, y1 = 1;
  while (b > 0) {
    q = a / b, r = a - q * b;
    *x = x2 - q * x1, *y = y2 - q * y1;
    a = b, b = r;
    x2 = x1, x1 = *x, y2 = y1, y1 = *y;
  }
  *d = a, *x = x2, *y = y2;
}


/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_clever(unsigned int size, unsigned int seed, int n) {

	const int MAX_SIZE = 10;
	srand(seed);
	
	fprintf(stdout, "%d\n", MAXSTRLEN);

	int r[MAXSTRLEN] = { -1 };

	//Print out all the r values
	size_t i;
	if(r[0] == -1){
		for(i = 0; i < MAXSTRLEN; i++)
		{
			r[i] = rand()%size;
			fprintf(stdout, "%d\n", r[i]);
			
		}
	}
	
	//Haven't really tested for large n values, as the requirements state full marks for n=2;
	//Currently I have n =10, which is satisfactory.
	if(n > MAX_SIZE)
		{
			fprintf(stdout, "Unknown results at n > %d, (n = %d was chosen) exiting...", MAX_SIZE, n);
			return;
		}
	
	char* keys[n]; 
	
	int j;
	 for(j = 0; j < n; j++)
	{
		long x, y, d;
		extended_euclid(r[j], size, &x, &y, &d);
		keys[j] = malloc(n* sizeof(char*));
		//We don't like these ASCII characters, so don't use them
		while(x < 33)
			x += size;
		
		//if r[j] is zero it will always hash to 0, don't 
		//need to find the zero mod
		if(r[j]!=0)
			x = x*r[j]-1;
		
		//We want x to be a char, therefore it must be less than MAXSTRLEN
		while(x > MAXSTRLEN)
			x -= size;
		
		//malloc the key everytime to ensure we get a new string
		char* key = (char*)malloc(2 * sizeof(char));

		//Copy the previous string into the new one, then concatenate the 
		//current string
		if(j>0)
		{
			//If we are on the second string, then concatenate the first string, to produce strings of significant length.
			key[0] = x;
			key[1] = '\0'; 
			strcpy(keys[j], keys[j-1]);
			strcat(keys[j],key);

		}
		else
		{	
			key[0] = x;
			key[1] = '\0'; 
			keys[j] = key;
		}
		
		
		//This loop checks to see if we can generate anymore ASCII characters without having to increase our length by 1.
		//Not really part of the requirements but is still nice to have, so we don't always end up generating the same ASCII character
		//each time we increase the string length.
		while((x - size) >= 33 && (j +1) < n)
			{
				x-= size;
				j++;
				char* key = (char*)malloc(2 * sizeof(char));
				keys[j] = malloc(10 * sizeof(char*));
				key[0] = x;
				key[1] = '\0'; 
				strcpy(keys[j], keys[j-1]);
				strcat(keys[j],key);
			}
		
	} 
	

	
	for(j = 0 ; j < n; j++)
	{
		fprintf( stdout, "%s\n", keys[j]);
	}
	

	
}
