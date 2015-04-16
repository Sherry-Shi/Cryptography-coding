// hex1.cpp : Defines the entry point for the console application.
//

// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include "stdio.h"
#include "hexdump.h"
#include "encode_base64.h"
#include "decode_base64.h"
 
using namespace std;
 
 
int main(int argc, char *argv[])
{
    int reading_from_file=0;  //reading from file or reading from stdin
    if (argc > 2) {          
         reading_from_file = 1;
    }
	// if the argument is malformed
	if(argc <= 1)
	{
		fputs("Malformed command & input from stdin.\n",stderr);
		return 1;
	}
	
	if((strcmp(argv[1], "hexdump") != 0) && (strcmp(argv[1], "enc-base64") != 0) && (strcmp(argv[1], "dec-base64") != 0)) 
	{
		fputs("Malformed command & input from stdin.\n",stderr);
		return 1;
	}
    FILE *fp=NULL;
    if (reading_from_file) {
		fp=fopen(argv[2], "rb"); 
        if (fp == NULL) {
               fprintf(stderr, "input file %s does not exist. Cannot open %s for reading.\n", argv[2],argv[2]); // can't open file
               return 0;
        }
           if(strcmp(argv[1], "hexdump") == 0)  hexdump(fp);   // if the argument is hexdump, then do hexdump
		   if(strcmp(argv[1], "enc-base64") == 0)  Encode_base64(fp);
		   if(strcmp(argv[1], "dec-base64") == 0)  Decode_base64(fp);
		   if((strcmp(argv[1], "hexdump") != 0) && (strcmp(argv[1], "enc-base64") != 0) && (strcmp(argv[1], "dec-base64") != 0)) 
		   {
				fputs("Malformed command & input from stdin",stderr);
				return 0;
		   }
            fclose(fp);
        } else {
           if(strcmp(argv[1], "hexdump") == 0) hexdump(stdin);  // read content from stdin , if the argument is hexdump, do hexdump
		   if(strcmp(argv[1], "enc-base64") == 0) Encode_base64(stdin);
		   if(strcmp(argv[1], "dec-base64") == 0) Decode_base64(stdin);
		   if((strcmp(argv[1], "hexdump") != 0) && (strcmp(argv[1], "enc-base64") != 0) && (strcmp(argv[1], "dec-base64") != 0)) 
		   {
				fputs("Malformed command & input from stdin",stderr);
				return 0;
		   }
        }
        return 0;
    }



