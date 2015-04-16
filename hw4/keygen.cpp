#include "hw4.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <openssl/md5.h> 
#include "stdlib.h"
using namespace std;

//get the random integer number
unsigned long RandomInteger(char *pphrase, int &count)
{
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
    int len1 = strlen(pphrase)+2+MD5_DIGEST_LENGTH;
    char *s = (char *)malloc(len1+1);
	char *output1 = (char *)malloc(8);
	unsigned char output[4];
    int i=0;
	int k = 0;
    MD5((unsigned char *)pphrase, strlen(pphrase), md5_buf);
    for (; k < count/2 + 1; k++){
        sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((unsigned char *)s, len1, md5_buf);
        if (++i == 100) i = 0;
    }
	memcpy(output1, md5_buf, sizeof(md5_buf)>>1);
	int  j = 0;
	if(!(count % 2)){
		for(char *pp = output1; pp < output1+4; pp++){
			output[j] = *pp;
			j++;
		}
	}else{
		for(char *pp = output1+4; pp < output1+8; pp++){
			output[j] = *pp;
			j++;
		}
	}
	char out[8];
	for(int m = 0; m < 4; m++){
		sprintf(out + m*2, "%02x", output[m]);
	}
	unsigned long random_integer;
	istringstream iss((const char*)out);
	iss >> hex >> random_integer;
	return random_integer;
    //free(s);
	//free(output1);
}
void swap(string &s, int i, int j){
	if(s.size() == 0) return;
	char tmp = s[i];
	s[i] = s[j];
	s[j] = tmp;
	return;
}

//generate each state
string stategen(char *pphrase, int &count){
	string state = "abcdefghijklmnopqrstuvwxyz";
	int L = 0, R = 0;
	unsigned long X = 0;
	for(int i = 0; i < 2; i++){
		L = 26;
		while(L > 1){
			X = RandomInteger(pphrase, count);
			count++;
			R = X%L;
			if(R != L - 1){
				swap(state, R, L-1);
			}
			L = L - 1;
		}
	}
	return state;

}
//generate the final keys with t period
void keygen(char *pphrase, int t){
	int i = 0,count = 0;
	string state = "";
	for(; i < t; i++){
		state = stategen(pphrase, count);
		printf("%s", state.c_str());
		printf("\n");
	}
	return;
}

