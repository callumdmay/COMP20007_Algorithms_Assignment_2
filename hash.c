/*
 * Callum May
   799309
*/
#include "hash.h"

#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 256

static int r[MAXSTRLEN] = { -1 };

/* Used as the second hashing function on double hash */
unsigned int linear_probe(void *e, unsigned int size) {
    
	return 1;
}

/* Very simple hash */
unsigned int worst_hash(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 0;
}

/* Basic numeric hash function */
unsigned int num_hash(long n, unsigned int size) {
    return n % size;
}

/* Bad hash function */
unsigned int bad_hash(char *key, unsigned int size) {
	
	static int a = -1;
	
	if(a == -1)
	{
		a = rand() % size;
	}
	
    return (a * key[0]) % size;
}

/* Universal hash function as described in Dasgupta et al 1.5.2 */
unsigned int universal_hash(unsigned char *key, unsigned int size) {
	
	
	//check to see if the r array has already been implemented, if it hasn't, then create it.
	//Only need to check the first array element, as it will always be >=0 if the universal hash has been created
	size_t i;
	if(r[0] == -1){
		for(i = 0; i < MAXSTRLEN; i++)
		{
			r[i] = rand()%size;
		}
	}
	
	int hashSum = 0;
	
	//Sum up all the hash elements
	for(i = 0 ; i < strlen(key); i++){
		hashSum += r[i] *(int)key[i];
	}
	
    return hashSum%size;
}
