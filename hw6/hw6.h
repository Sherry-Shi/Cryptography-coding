#include "stdio.h"
#include <string>
using namespace std;
#define NK 4
#define NB 4
#define NR 10

int tablecheck(FILE *fp);
void modprod(long unsigned p1, long unsigned p2);
void crypt(unsigned char key[], FILE* ifp, int mode);
long unsigned inverse(long unsigned p);
unsigned char xtime(unsigned char c);
void modProduct(unsigned char a[], unsigned char b[], unsigned char d[]);
void keyexpand(unsigned char key[]);
void RotWord(unsigned char a[], int len);
void subWord(unsigned char tmp[], int len);

extern unsigned char S[256];
extern unsigned char P[4];
extern unsigned char INVP[4];
