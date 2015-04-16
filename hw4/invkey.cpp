#include "hw4.h"
#include <stdlib.h>

int findChar(char * input, char c){
	for(int i = 0; i < 26; i++){
		if(input[i] == c) return i;
	}
	return -1;
}
void invkey(FILE *keyfile){
	char output[27]={'a'};
	char line[30]={'a'};
	int index = 0;
	string origin = "abcdefghijklmnopqrstuvwxyz";
	while(!feof(keyfile)){
		if(fgets(line, 30, keyfile) != NULL){ 
			//int len = strlen(line);
			for(int i = 0; i < 26; i++){
				index = findChar(line, origin[i]);
				output[i] = origin[index];
			}
			printf("%s", output);
			printf("\n");
		}
	}
	fclose(keyfile);
}
