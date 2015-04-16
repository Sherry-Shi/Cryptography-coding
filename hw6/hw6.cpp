// DES.cpp : Defines the entry point for the console application.
#include "hw6.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "unistd.h"
#include <string>
using namespace std;
int CryptCheck(char *argv[], int argc, int name){
	if(argc < 4 || argc > 5 )
		{
			fprintf(stderr, "Error: malformed command'\n");
			return 0;
		}
		if(argv[2][0] != '-' || argv[3][0] != '-')
		{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		char c;
		FILE *fp =NULL;
		char* fileName;
		char high = 0;
		char low = 0;
		unsigned char *key = NULL;
		for(int i = 2; i < 4; i++){
			c = argv[i][1];
			char* src = NULL;
			if(c == 'k'){
				key = (unsigned char *)malloc(sizeof(char) * 16);
				//keycheck
				src = argv[i] + 3;
				if (strlen(src) != 32) {
					fprintf(stderr, "Error: The key should be 16 bytes\n" );
					return 0;
				}
				for (int k = 0; k < 32; k+=2) {
					if (src[k] >= '0' && src[k] <= '9' ) {
						high = src[k] - '0';
					} else if (src[k] >= 'a' && src[k] <= 'f') {
						high = src[k] - 'a' + 0xa;
					}else {
						fprintf(stderr, "%c is not a hex charactrer!\n",src[k]);
						return 0;
					}
					if (src[k+1] >= '0' && src[k+1] <= '9' ) {
						low = src[k+1] - '0';
					} else if (src[k+1] >= 'a' && src[k+1] <= 'f') {
						low = src[k+1] - 'a' + 0xa;
					}else {
						fprintf(stderr, "%c is not a hex charactrer!\n",src[k+1]);
						return 0;
					}
					key[k/2] = (high << 4) + low;
				}
			}else {
				if (c == 't') {
					fileName = argv[i] + 3;
					if(access (fileName, F_OK) != -1){
						fp = fopen(fileName, "r");
						if(fp == NULL){
							fprintf(stderr, "Error: Can't open %s\n", fileName);
							return 0;
						}
					}else{
						fprintf(stderr, "Error: Can't open input file. File doesn't exist!\n");
						return 0;
					}
					//table check
					if(!tablecheck(fp)){
						return 0;
					}
				}
				else {
					fprintf(stderr, "Error: Wrong option\n");
					return 0;
				}
			}
		}
		FILE *fp1 = stdin;
		if(argc == 5)
		{
			fileName = argv[4];
				if(access (fileName, F_OK) != -1){
					fp1 = fopen(fileName, "r");
					if(fp1 == NULL){
					fprintf(stderr, "Error: Can't open %s\n", fileName);
						return 0;
					}
				}else{
					fprintf(stderr, "Error: Can't open input fileFile doesn't exist!\n");
					return 0;
				}
		}
		//long long unsigned k = 0;
		if (name == 0) {
			crypt(key, fp1, 0);
		}else {
			crypt(key, fp1, 1);
		}
		return 1;
}

int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	char * table_file = NULL;
	char c = 0;
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
	if(strcmp(argv[1],"tablecheck") == 0)   //if the command is "tablecheck"
	{
		if(argc != 3 )
		{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		if(argv[2][0] != '-')
		{
			fprintf(stderr, "Error:malformed command\n");
			return 0;
		}
		c = argv[2][1];
		if (c == 't'){
				table_file = argv[2] + 2;
		}else {
				fprintf(stderr, "Error: malformed command, invalid command option\n");
				return 0;
		}
		if(table_file[0] != '=')
		{
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		if(access (argv[2]+3, F_OK) != -1){
			fp = fopen(argv[2]+3, "r");
			if(fp == NULL){
				fprintf(stderr, "Error: Can't open %s\n", argv[2]+3);
				return 0;
			}
		}else{
			fprintf(stderr, "Error: Can't open input file. File doesn't exist!\n");
			return 0;
		}
		tablecheck(fp);
		return 1;
	}
	if (strcmp(argv[1], "modprod") == 0) {
		if (argc != 4) {
			fprintf(stderr, "Error: malformed command\n");
			return 0;
		}
		unsigned poly1 = 0;
		unsigned poly2 = 0;
		char hex = 0;
		for (int i = 2; i < argc; i++) {
			if(argv[i][0] != '-') {
				fprintf(stderr, "Error:malformed command\n");
				return 0;
			}
			char sp = argv[i][1];
			//fprintf(stderr, "%s\n", sp);
			if (sp == 'p' && argv[i][2]== '1'){
				if (*(argv[i] + 12) != 0) {
					fprintf(stderr, "Error: wrong format of polynomial in p1.\n" );
					return 0;
				}
				for (int j = 0; j < 8; j++) {
					poly1 = poly1 << 4;
					char tmp_c = *(argv[i] + 4 + j);
					if (tmp_c >='0' && tmp_c <= '9') {
						hex = tmp_c - '0';
					} else if (tmp_c >= 'a' && tmp_c <= 'f') {
						hex = tmp_c - 'a' + 0xa;
					} else {
						fprintf(stderr, "Error: %c is not a valid hex character!\n", tmp_c);
						return 0;
					}
					poly1 += hex;
				}

			}else if (sp == 'p' && argv[i][2] == '2') {
				if (*(argv[i] + 12) != 0) {
					fprintf(stderr, "Error: wrong format of polynomial in p1.\n" );
					return 0;
				}
				for (int j = 0; j < 8; j++) {
					poly2 = poly2 << 4;
					char tmp_c = *(argv[i] + 4 + j);
					if (tmp_c >='0' && tmp_c <= '9') {
						hex = tmp_c - '0';
					} else if (tmp_c >= 'a' && tmp_c <= 'f') {
						hex = tmp_c - 'a' + 0xa;
					} else {
						fprintf(stderr, "Error: %c is not a valid hex character!\n", tmp_c);
						return 0;
					}
					poly2 += hex;
				}

			}else{
				fprintf(stderr, "Error: malformed command, invalid command option\n");
				return 0;
			}
		}
		modprod(poly1, poly2);
		return 1;
	}
	if (strcmp(argv[1], "keyexpand") == 0) {
		if (argc != 4) {
			fprintf(stderr, "Error: Wrong number of arguments for the command.\n");
			return 0;
		}
		unsigned char *key = NULL;
		char high = 0;
		char low = 0;
		char* fileName;
		for(int i = 2; i < 4; i++){
			c = argv[i][1];
			char* src = NULL;
			if(c == 'k'){
				key = (unsigned char *)malloc(sizeof(char) * 16);
				//keycheck
				src = argv[i] + 3;
				if (strlen(src) != 32) {
					fprintf(stderr, "Error: The key should be 16 bytes\n" );
					return 0;
				}
				for (int k = 0; k < 32; k+=2) {
					if (src[k] >= '0' && src[k] <= '9' ) {
						high = src[k] - '0';
					} else if (src[k] >= 'a' && src[k] <= 'f') {
						high = src[k] - 'a' + 0xa;
					}else {
						fprintf(stderr, "%c is not a hex charactrer!\n",src[k]);
						return 0;
					}
					if (src[k+1] >= '0' && src[k+1] <= '9' ) {
						low = src[k+1] - '0';
					} else if (src[k+1] >= 'a' && src[k+1] <= 'f') {
						low = src[k+1] - 'a' + 0xa;
					}else {
						fprintf(stderr, "%c is not a hex charactrer!\n",src[k+1]);
						return 0;
					}
					key[k/2] = (high << 4) + low;
				}
			}else {
				if (c == 't') {
					fileName = argv[i] + 3;
					if(access (fileName, F_OK) != -1){
						fp = fopen(fileName, "r");
						if(fp == NULL){
							fprintf(stderr, "Error: Can't open %s\n", fileName);
							return 0;
						}
					}else{
						fprintf(stderr, "Error: Can't open input file. File doesn't exist!\n");
						return 0;
					}
					//table check
					if(!tablecheck(fp)){
						return 0;
					}
				}
				else {
					fprintf(stderr, "Error: Wrong option\n");
					return 0;
				}
			}
		}
		keyexpand(key);
		return 1;
	}
	if(strcmp(argv[1],"encrypt") == 0)      //if the command is "encrypt"
	{
		CryptCheck(argv, argc, 0);
		return 1;
	}
	if(strcmp(argv[1],"decrypt") == 0)          //if the command is invkey
	{	
		CryptCheck(argv, argc, 1);
		return 1;
	}
	if (strcmp(argv[1],"inverse") == 0) {
		if (argc != 3) {
			fprintf(stderr, "Error: wrong number of arguments for inverse.\n");
			return 0;
		}
		char* s_c = argv[2] + 1;
		long unsigned poly = 0;
		char hex = 0;
		if (strcmp(s_c, "-p") == 0) {
			if (*(argv[2] + 11) != 0) {
				fprintf(stderr, "Error: Wrong format of polynomial\n");
				return 0;
			}else{

				for (int j = 0; j < 8; j++) {
					poly = poly << 4;
					char tmp_c1 = argv[2][3+j];
					if (tmp_c1 >='0' && tmp_c1 <= '9') {
						hex = tmp_c1 - '0';
					} else if (tmp_c1 >= 'a' && tmp_c1 <= 'f') {
						hex = tmp_c1 - 'a' + 0xa;
					} else {
						fprintf(stderr, "Error: %c is not a valid hex character!\n", tmp_c1);
						return 0;
					}
					poly += hex;
				}
			}
		}
		else {
			fprintf(stderr, "Error: wrong command!\n");
			return 0;
		}
	//	inverse(poly);
	}
	else{
			fprintf(stderr, "Error: malformed command!\n");
			return 0;
	}
	return 1;
}
