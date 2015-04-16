// hw4.cpp : Defines the entry point for the console application.
//
#include "hw4.h"

//check if the keyfile is not malformed keyfile
bool isValidKeyfile(FILE* keyfile){
	int len = 0;
	map<char, int> char_map;
	char line[30]={'a'};
	while(!feof(keyfile)){
		
		if(fgets(line, 30, keyfile) != NULL){
			len = strlen(line);
			char_map.clear();
			if(!feof(keyfile)){
				for(int i = 0; i < len - 2; i++){
					if(line[i] >= 'a' && line[i] <='z'){
						if(char_map.count(line[i]) > 0){
							return false;
						}else{
							char_map[line[i]] = 1;
						}
					}else{
						
						return false;
					}
				}
			}
		}
					
	}
		return true;
}
int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	char *pphrase = NULL;
	char * fileName = 0;
	FILE *fp1 = stdin;
	char * slen = NULL;
	int maxlen = 0;
	//char *pphrase = NULL;
	char c = 0;
	int len = 0;
	if(argc < 2)
	{
		fprintf(stderr, "Error:malformed command.\n");
		return 0;
	}
	if(argc > 5)
	{
		fprintf(stderr, "Error: malformed command.\n");
		return 0;
	}
	if(strcmp(argv[1],"keygen") == 0)   //if the command is "keygen"
	{
		if(argc != 4 )
		{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		if(argv[2][0] != '-' && argv[3][0] != '-')
		{
			fprintf(stderr, "Error:malformed command\n");
			return 0;
		}
		for(int i = 2; i < 4;i++)
		{
			c = argv[i][1];
			if(c == 'p')
			{
				pphrase = argv[i] + 2;
				continue;
			}
			else if(c == 't')
			{
				slen = argv[i] + 2;
				continue;
			}
			else 
			{
				fprintf(stderr, "Error: malformed command, invalid command option\n");
				return 0;
			}
		}
		if(pphrase[0] != '=' &&slen[0] != '=')
		{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		if(strcmp(argv[3] + 3, "") == 0){
			fprintf(stdout, "Error: malformed command\n");
			return 0;
		}
		if(sscanf(argv[3] + 3, "%d", &len) == EOF){
			fprintf(stdout, "Error: malformed command! Can't convert %s into integer\n", argv[2] + 3);
			return 0;
		}
		keygen(pphrase + 1, len);
		return 1;
	}
	if(strcmp(argv[1],"crypt") == 0)      //if the command is "crypt"
	{
		FILE* keyfile;
		if(argc < 3 || argc > 4 )
		{
			fprintf(stderr, "Error: malformed command'\n");
			return 0;
		}
		if(argv[2][0] != '-')
		{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		c = argv[2][1];
		if(c == 'k')
		{
			fileName = argv[2]+3;
			if(access (fileName, F_OK) != -1){
				keyfile = fopen(fileName, "r");
				if(keyfile == NULL){
					fprintf(stderr, "Error: Can't open %s\n", fileName);
					return 0;
				}
			}else{
				fprintf(stderr, "Error: File doesn't exist!\n");
				return 0;
			}
			if(!isValidKeyfile(keyfile)){
				fprintf(stderr, "Error: Malformed Key File!\n");
				return 0;
			}
			
		}
		else 
		{
			fprintf(stderr, "Error: malformed command, invalid command option\n");
			return 0;
		}
		if(argc == 4)
		{
			fileName = argv[3];
				if(access (fileName, F_OK) != -1){
					fp = fopen(fileName, "r");
					if(fp == NULL){
						fprintf(stderr, "Error: Can't open %s\n", fileName);
						return 0;
					}
				}else{
					fprintf(stderr, "File doesn't exist!\n");
					return 0;
				}
		}
		fseek(keyfile, 0, SEEK_SET);	
		crypt(keyfile, fp);
		return 1;
	}
	if(strcmp(argv[1],"invkey") == 0)          //if the command is invkey
	{	
		if(argc != 3){
			fprintf(stderr, "Error: malformed command!\n");
			return 0;
		}
		fileName = argv[2];
		if(access (argv[2], F_OK) != -1){
				fp1 = fopen(argv[2], "r");
				if(fp1 == NULL){
					fprintf(stderr, "Error: Can't open %s\n", fileName);
					return 0;
				}
		}else{
				fprintf(stderr, "File doesn't exist!\n");
				return 0;
		}
		if(!isValidKeyfile(fp1)){
				fprintf(stderr, "Error: Malformed Key File!\n");
				return 0;
		}	
		fseek(fp1, 0, SEEK_SET);		
		invkey(fp1);
		return 1;
	}
	if(strcmp(argv[1], "histo") == 0){             //if the command is histo
		int period = 0;
		int which = 0;
		if(argc < 4 || argc > 5){
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		
		}else{
			for(int i = 2; i < 4; i++){
				c = argv[i][1];
				if(c == 't'){
					if(strcmp(argv[i] + 3, "") == 0){
						fprintf(stderr, "Error: malformed command\n");
						return 0;
					}
					if(sscanf(argv[i] + 3, "%d", &period) == EOF){
						fprintf(stdout, " malformed command! Can't convert %s into integer\n", argv[i+2] + 3);
						return 0;
					}
				}else if(c == 'i'){
					if(strcmp(argv[i] + 3, "") == 0){
						fprintf(stderr, "Error: malformed command\n");
						return 0;
					}
					if(sscanf(argv[i] + 3, "%d", &which) == EOF){
						fprintf(stdout, "malformed command! Can't convert %s into integer\n", argv[i+2] + 3);
						return 0;
					}
				}else{
					fprintf(stderr, "Error: malformed command\n");
					return 0;
				}
			}
			if(argc == 5){
				fileName = argv[4];
				if(access (argv[4], F_OK) != -1){
					fp = fopen(argv[4], "r");
					if(fp == NULL){
						fprintf(stderr, "Error: Can't open %s\n", fileName);
						return 0;
					}
				}else{
					fprintf(stderr, "File doesn't exist!\n");
					return 0;
				}	
			}
		}
		histo(period, which, fp);
		return 1;
	}
	if(strcmp(argv[1], "solve") == 0)              //if the command is solve
	{
		if(argc == 4)
		{
			if(argv[2][1] == 'l'){
				if(strcmp(argv[2] + 3, "") == 0){
					fprintf(stdout, "Error: malformed command!\n");
					return 0;
				}
				if(sscanf(argv[2] + 3, "%d", &maxlen) == EOF){
					fprintf(stdout, "Error: malformed command! Can't convert %s into integer\n", argv[2] + 3);
					return 0;
				}
				fileName = argv[3];
				if(access (argv[3], F_OK) != -1){
					
					fp = fopen(argv[3], "r");
					if(fp == NULL){
						fprintf(stderr, "Error: Can't open %s\n", fileName);
						return 0;
					}
				}else{
					fprintf(stderr, "File doesn't exist!\n");
					return 0;
				}	
			}else{
				fprintf(stderr, "Error: malformed command!\n");
				return 0;
			}
		}
		else{
			fprintf(stderr, "Error: malformed command\n'");
			return 0;
		}
		solve(maxlen, fp);
		return 1;
	}
	else
	{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
	}
}













