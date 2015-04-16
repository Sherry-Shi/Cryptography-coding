#include "hw6.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "unistd.h"
#include <map>
#include <string>
using namespace std;
unsigned char S[256];
unsigned char P[4];
unsigned char INVP[4];

void reverse(unsigned char a[], int left, int right);

int fillTable(char* src, unsigned char* table, int len) {
	if (strlen(src) != len) {
		fprintf(stderr, "Error: The length of polynomial should be %d\n", len);
		return -1;
	}
	//printf("src = %s\n", src);
	char high = 0;
	char low = 0;
	for (int k = 0; k < len; k+=2) {
		if (src[k] >= '0' && src[k] <= '9' ) {
			high = src[k] - '0';
		} else if (src[k] >= 'a' && src[k] <= 'f') {
			high = src[k] - 'a' + 0xa;
		}else {
			if (src[k] = ' ') {
				fprintf(stderr, "Error: Wrong number of entries in S.\n");
				return -1;
			}
			fprintf(stderr, "Error: %c is not a hex charactrer!\n",src[k]);
			return -1;
		}
		if (src[k+1] >= '0' && src[k+1] <= '9' ) {
			low = src[k+1] - '0';
		} else if (src[k+1] >= 'a' && src[k+1] <= 'f') {
			low = src[k+1] - 'a' + 0xa;
		}else {
			fprintf(stderr, "Error: %c is not a hex charactrer!\n",src[k+1]);
			return -1;
		}
		table[k/2] = (high << 4) + low; 
	}
	return 1;
}
int tablecheck(FILE *fp) {
	//CHECK S
	char buff[1024];
	if (fgets(buff, 1024, fp) == NULL) {
		fprintf(stderr, "Error: Cann't read S-box\n" );
		return -1;
	}
	if (strlen(buff) != 515) {
		fprintf(stderr, "Error: Wrong numbers of values\n");
		return -1;
	}

	if (buff[0] != 'S' || buff[1] != '=') {
		fprintf(stderr, "Error: Should start with \"S=\"\n");
		return -1;
	}
	
	buff[514] = 0;
	int flag = fillTable(buff+2, S, 512);
	if (flag == -1) return -1;
	map<int, int> check;
	for (int i = 0; i < 256; i++) {
		if (check[S[i]] == 0) {
			check[S[i]]++;
		}else {
			fprintf(stderr, "Error: there are repeated elements in S.\n");
			return -1;
		}
	}
	
	
	//check P
	char buff1[20];
	if (fgets(buff1, 20, fp) == NULL) {
		fprintf(stderr, "Error: There is no P= \n");
		return -1;
	}
	if (strlen(buff1 + 2) != 9) {
		fprintf(stderr, "Error: Wrong number of values in P\n");
		return -1;
	}
	if (buff1[0] != 'P' || buff1[1] != '=') {
		fprintf(stderr, "Error: Should start with \"P=\"\n");
		return -1;
	}
	buff1[10] = 0;
	flag = fillTable(buff1+2, P, 8);
	if (flag == -1) return -1;
	//check INVP
	char buff2[20];
	if (fgets(buff2, 20, fp) == NULL) {
		fprintf(stderr, "Error: There is no INVP \n");
		return -1;
	}
	if (strlen(buff2 + 5) != 9) {
		fprintf(stderr, "Error: Wrong number of values in INVP\n");
		return -1;
	}
	string strr(buff2);
	string tttmp = strr.substr( 0, 5);
	if (strcmp (tttmp.c_str(), "INVP=") != 0) {
		fprintf(stderr, "Error: Should start with \"INVP=\"\n");
		return -1;
	}
	buff2[13] = 0;
	flag = fillTable(buff2+5, INVP, 8);
	if (flag == -1) return -1;
	//check INVP
	reverse(P, 0, 3);
	reverse(INVP, 0, 3);
	unsigned char d[4];
	modProduct(P, INVP, d);
	if (d[3] || d[2] || d[1] || d[0] != 1) {
		fprintf(stderr, "Error: INVP is not inverse of P\n");
		return -1;
	}

	return 1;
}

void reverse(unsigned char a[], int left, int right) {
	if (left >= right) {
		return;
	}
	unsigned char tmp = 0;
	tmp = a[left];
	a[left] = a[right];
	a[right] = tmp;
	reverse(a, left+1, right-1);
}

